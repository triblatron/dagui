#include "config/config.h"
#include "core/Node.h"
#include "core/Graph.h"
#include "MemoryNodeLibrary.h"
#include "core/SignalPath.h"
#include "NodeEditorLive.h"

#include "node_editor.h"

#include <imnodes.h>
#include <imgui.h>

#include <chrono>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <vector>

namespace example
{
namespace
{
template<class T>
T clamp(T x, T a, T b)
{
    return std::min(b, std::max(x, a));
}

static float current_time_seconds = 0.f;
static bool  emulate_three_button_mouse = false;

ImU32 evaluate(const dagbase::Graph& graph, const int root_node)
{
    return 0;
}

namespace propertyeditor
{
    class NodePropertyInspector
    {
    public:
        NodePropertyInspector()
        {
        }

        ~NodePropertyInspector()
        {
            // Do nothing.
        }

        void editNode(dagbase::Node* node)
        {
            _node = node;
        }

        void show()
        {
            bool open = true;
            // Main body of the Demo window starts here.
            //if (!ImGui::Begin("Node Property Editor Demo", &open, 0))
            //{
            //    // Early out if the window is collapsed, as an optimization.
            //    ImGui::End();
            //    return;
            //}

            if (_node)
            {
                _node->edit(ImGui::GetCurrentContext());
            }
            //ImGui::End();
        }
    private:
        dagbase::Node* _node{ nullptr };
    };
}

class DagNodeEditor
{
public:
    DagNodeEditor()
        : root_node_id_(-1),
          minimap_location_(ImNodesMiniMapLocation_BottomRight)
    {
        // Do nothing.
    }

    void show()
    {
        // Update timer context
        current_time_seconds = timeSinceInit();

        auto flags = ImGuiWindowFlags_MenuBar;

        // The node editor window
        ImGui::Begin("dag node editor", NULL, flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Mini-map"))
            {
                const char* names[] = {
                    "Top Left",
                    "Top Right",
                    "Bottom Left",
                    "Bottom Right",
                };
                int locations[] = {
                    ImNodesMiniMapLocation_TopLeft,
                    ImNodesMiniMapLocation_TopRight,
                    ImNodesMiniMapLocation_BottomLeft,
                    ImNodesMiniMapLocation_BottomRight,
                };

                for (int i = 0; i < 4; i++)
                {
                    bool selected = minimap_location_ == locations[i];
                    if (ImGui::MenuItem(names[i], NULL, &selected))
                        minimap_location_ = locations[i];
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Style"))
            {
                if (ImGui::MenuItem("Classic"))
                {
                    ImGui::StyleColorsClassic();
                    ImNodes::StyleColorsClassic();
                }
                if (ImGui::MenuItem("Dark"))
                {
                    ImGui::StyleColorsDark();
                    ImNodes::StyleColorsDark();
                }
                if (ImGui::MenuItem("Light"))
                {
                    ImGui::StyleColorsLight();
                    ImNodes::StyleColorsLight();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::TextUnformatted("Edit the color of the output color window using nodes.");
        ImGui::BeginTable("top",3);
        ImGui::TableSetupColumn("help");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextUnformatted("A -- add node");
        ImGui::TextUnformatted("X -- delete selected node or link");
        ImGui::TextUnformatted("G -- group selected nodes");
        ImGui::TableSetColumnIndex(1);
//        ImGui::TableSetupColumn("nodes");
        ImGui::TableNextRow();
        if (ImGui::Checkbox("emulate_three_button_mouse", &emulate_three_button_mouse))
        {
            ImNodes::GetIO().EmulateThreeButtonMouse.Modifier =
                emulate_three_button_mouse ? &ImGui::GetIO().KeyAlt : NULL;
        }

        ImNodes::BeginNodeEditor();

        // Handle new nodes
        // These are driven by the user, so we place this code before rendering the nodes
        {
            const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                                    ImNodes::IsEditorHovered() && ImGui::IsKeyReleased(ImGuiKey_A);

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
            if (!ImGui::IsAnyItemHovered() && open_popup)
            {
                ImGui::OpenPopup("add node");
            }

            if (ImGui::BeginPopup("add node"))
            {
                const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

                int i = 0;
                nodeEditor_.eachClass([this,&i](dagbase::Node& node) {
                    std::string label = node.className();
                    ImGui::PushID(i++);
                    if (ImGui::MenuItem(label.c_str()))
                    {
                        auto status = nodeEditor_.createNode(node.className(), node.name());
                        if (status.status == dagbase::Status::StatusCode::STATUS_OK)
                        {
//                            graph_.addNode(created);
                        }
                    }
                    ImGui::PopID();
                    return true;
                    });
                ImGui::EndPopup();

            }
            ImGui::PopStyleVar();
        }

        // Draw all the nodes
        nodeEditor_.eachNode([this](dagbase::Node* node) {
            ImNodes::BeginNode(node->id());
            ImNodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node->name().c_str());
            ImNodes::EndNodeTitleBar();
            ImGui::Dummy(ImVec2(100.0f, 0.0f));
            ImGui::Spacing();
            
            for (std::size_t portIndex = 0; portIndex < node->totalPorts(); ++portIndex)
            {
                auto port = node->dynamicPort(portIndex);
                switch (port->dir())
                {
                case dagbase::PortDirection::DIR_OUT:
                    ImNodes::BeginOutputAttribute(port->id());
                    ImGui::Indent(100.0 - ImGui::CalcTextSize(port->name().c_str())[0]);
                    ImGui::TextUnformatted(port->name().c_str());
                    ImNodes::EndOutputAttribute();
                    break;
                case dagbase::PortDirection::DIR_IN:
                    ImNodes::BeginInputAttribute(port->id());
                    ImGui::TextUnformatted(port->name().c_str());
                    ImNodes::EndInputAttribute();
                    break;
                    case dagbase::PortDirection::DIR_INTERNAL:
                    ImNodes::BeginOutputAttribute(port->id());
                    ImGui::TextUnformatted(port->name().c_str());
                    ImNodes::EndStaticAttribute();
                    break;
                }
            }
            ImNodes::EndNode();

            return true;
            });
        
        nodeEditor_.eachSignalPath([this](dagbase::SignalPath* signalPath) {
            ImNodes::Link(signalPath->id(), signalPath->source()->id(), signalPath->dest()->id());
            return true;
            });
        //for (const auto& edge : graph_.edges())
        //{
        //    // If edge doesn't start at value, then it's an internal edge, i.e.
        //    // an edge which links a node's operation to its input. We don't
        //    // want to render node internals with visible links.
        //    if (graph_.node(edge.from).type != NodeType::value)
        //        continue;

        //    ImNodes::Link(edge.id, edge.from, edge.to);
        //}

        ImNodes::MiniMap(0.2f, minimap_location_);
        ImNodes::EndNodeEditor();

        // Handle new links
        // These are driven by Imnodes, so we place the code after EndNodeEditor().

        {
            int start_attr, end_attr;
            if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
            {
                nodeEditor_.connect(start_attr, end_attr);
            }
        }

        // Handle deleted links

        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                nodeEditor_.disconnect(link_id);
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedLinks();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_links;
                selected_links.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedLinks(selected_links.data());
                for (const int edge_id : selected_links)
                {
                    nodeEditor_.disconnect(edge_id);
                }
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedNodes();
            if (num_selected > 0 && ImGui::IsKeyReleased(ImGuiKey_X))
            {
                static std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                for (const int node_id : selected_nodes)
                {
                    nodeEditor_.deleteNode(node_id);
                }
            }
        }

        {
            const int num_selected = ImNodes::NumSelectedNodes();
            if (num_selected > 0)
            {
                std::vector<int> selected_nodes;
                selected_nodes.resize(static_cast<size_t>(num_selected));
                ImNodes::GetSelectedNodes(selected_nodes.data());
                dag::SelectionInterface::Cont a;
                for (auto node_id : selected_nodes)
                {
                    if (auto node = nodeEditor_.activeGraph()->node(node_id); node)
                        a.m.emplace(node);
                }
                nodeEditor_.select(dag::NodeEditorInterface::SELECTION_SET, a);
            }
            else
            {
                nodeEditor_.selectNone();
            }
        }
        if (ImGui::IsKeyReleased(ImGuiKey_G))
        {
            if (ImNodes::NumSelectedNodes() > 0)
            {
                nodeEditor_.createChild();
            }
        }

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(2);
        // Allow a single selection to be edited
        if (ImNodes::NumSelectedNodes() == 1)
        {
            int selectedNodeId{ -1 };
            ImNodes::GetSelectedNodes(&selectedNodeId);
            propertyEditor_.editNode(nodeEditor_.activeGraph()->node(selectedNodeId));
        }
        this->propertyEditor_.show();
        ImGui::EndTable();
        ImGui::End();
        // The color output window
        const ImU32 color =
            root_node_id_ != -1 ? evaluate(*nodeEditor_.activeGraph(), root_node_id_) : IM_COL32(255, 20, 147, 255);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
        ImGui::Begin("output color");
        ImGui::End();
        ImGui::PopStyleColor();
    }

    void setInitTick(std::chrono::high_resolution_clock::time_point tick)
    { 
        initTick_ = tick;
    }

    double timeSinceInit() 
    {
        const auto interval = std::chrono::duration_cast<std::chrono::seconds>
                              (std::chrono::high_resolution_clock::now() - initTick_);

        return interval.count();
    }
private:
    dagbase::NodeID        root_node_id_;
    ImNodesMiniMapLocation minimap_location_;
    std::chrono::high_resolution_clock::time_point initTick_;
    dag::NodeEditorLive nodeEditor_;
    propertyeditor::NodePropertyInspector propertyEditor_;
};

static DagNodeEditor color_editor;
} // namespace

void NodeEditorInitialize()
{
    ImNodesIO& io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
    color_editor.setInitTick(std::chrono::high_resolution_clock::now());
}

void NodeEditorShow() { color_editor.show(); }

void NodeEditorShutdown() {}
} // namespace example
