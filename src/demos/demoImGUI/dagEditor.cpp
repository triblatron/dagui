#include "config/config.h"
#include "core/Node.h"
#include "core/Graph.h"
#include "MemoryNodeLibrary.h"
#include "core/SignalPath.h"

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
            if (!ImGui::Begin("Node Property Editor Demo", &open, 0))
            {
                // Early out if the window is collapsed, as an optimization.
                ImGui::End();
                return;
            }

            if (_node)
            {
                _node->edit(ImGui::GetCurrentContext());
            }
            ImGui::End();
        }
    private:
        dagbase::Node* _node{ nullptr };
    };
}

class DagNodeEditor
{
public:
    DagNodeEditor()
        : graph_(), root_node_id_(-1),
          minimap_location_(ImNodesMiniMapLocation_BottomRight)
    {
        graph_.setNodeLibrary(&nodeLib_);
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
        ImGui::Columns(2);
        ImGui::TextUnformatted("A -- add node");
        ImGui::TextUnformatted("X -- delete selected node or link");
        ImGui::NextColumn();
        if (ImGui::Checkbox("emulate_three_button_mouse", &emulate_three_button_mouse))
        {
            ImNodes::GetIO().EmulateThreeButtonMouse.Modifier =
                emulate_three_button_mouse ? &ImGui::GetIO().KeyAlt : NULL;
        }
        ImGui::Columns(2);

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
                nodeLib_.eachNode([this,&i](dagbase::Node& node) {
                    std::string label = node.className();
                    ImGui::PushID(i++);
                    if (ImGui::MenuItem(label.c_str()))
                    {
                        auto created = graph_.createNode(node.className(), node.name());
                        if (created)
                        {
                            graph_.addNode(created);
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
        graph_.eachNode([this](dagbase::Node* node) {
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
                }
            }
            ImNodes::EndNode();

            return true;
            });
        
        graph_.eachSignalPath([this](dagbase::SignalPath* signalPath) {
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
                const auto start_type = graph_.port(start_attr)->dir();
                const auto end_type = graph_.port(end_attr)->dir();

                const bool valid_link = start_type != end_type;
                if (valid_link)
                {
                    // Ensure the edge is always directed from the value to
                    // whatever produces the value
                    if (start_type != dagbase::PortDirection::DIR_OUT)
                    {
                        std::swap(start_attr, end_attr);
                    }
                    graph_.addSignalPath(new dagbase::SignalPath(graph_.port(start_attr), graph_.port(end_attr)));
                }
            }
        }

        // Handle deleted links

        {
            int link_id;
            if (ImNodes::IsLinkDestroyed(&link_id))
            {
                graph_.removeSignalPath(graph_.signalPath(link_id));
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
                    graph_.removeSignalPath(graph_.signalPath(edge_id));
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
                dagbase::NodeArray nodes;
                graph_.findAllNodes(&nodes);
                for (const int node_id : selected_nodes)
                {
                    graph_.removeNode(graph_.node(node_id));
                    //auto iter = std::find_if(
                    //    nodes.begin(), nodes.end(), [node_id](const dagbase::Node& node) -> bool {
                    //        return node.id() == node_id;
                    //    });
                    // Erase any additional internal nodes
                    //switch (iter->type)
                    //{
                    //case UiNodeType::add:
                    //    graph_.erase_node(iter->ui.add.lhs);
                    //    graph_.erase_node(iter->ui.add.rhs);
                    //    break;
                    //case UiNodeType::multiply:
                    //    graph_.erase_node(iter->ui.multiply.lhs);
                    //    graph_.erase_node(iter->ui.multiply.rhs);
                    //    break;
                    //case UiNodeType::output:
                    //    graph_.erase_node(iter->ui.output.r);
                    //    graph_.erase_node(iter->ui.output.g);
                    //    graph_.erase_node(iter->ui.output.b);
                    //    root_node_id_ = -1;
                    //    break;
                    //case UiNodeType::sine:
                    //    graph_.erase_node(iter->ui.sine.input);
                    //    break;
                    //default:
                    //    break;
                    //}
                    //graph_.removeNode(*iter);
                }
            }
        }

        ImGui::End();
        ImGui::NextColumn();
        // Allow a single selection to be edited
        if (ImNodes::NumSelectedNodes() == 1)
        {
            int selectedNodeId{ -1 };
            ImNodes::GetSelectedNodes(&selectedNodeId);
            propertyEditor_.editNode(graph_.node(selectedNodeId));
        }
        this->propertyEditor_.show();
        // The color output window
        const ImU32 color =
            root_node_id_ != -1 ? evaluate(graph_, root_node_id_) : IM_COL32(255, 20, 147, 255);
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
    dagbase::Graph         graph_;
    dagbase::NodeID        root_node_id_;
    ImNodesMiniMapLocation minimap_location_;
    std::chrono::high_resolution_clock::time_point initTick_;
    dag::MemoryNodeLibrary nodeLib_;
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
