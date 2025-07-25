//
// Created by tony on 19/08/24.
//

#include "config/config.h"

#include "core/Window.h"
#include "util/enums.h"
#include "util/Searchable.h"
#include "core/ShapeFactory.h"

namespace dagui
{
    Window::Window(Widget *parent)
    :
    Widget(dagbase::Atom::intern("Window"),parent)
    {
        // Do nothing
    }

    void
    Window::configure(dagbase::ConfigurationElement &config, WidgetFactory &factory, dagui::ShapeFactory &shapeFactory)
    {
        Widget::configure(config, factory, shapeFactory);

        if (auto element=config.findElement("title"); element)
        {
            _title = element->asString();
        }
    }

    dagbase::ConfigurationElement::ValueType Window::find(std::string_view path) const
    {
        dagbase::ConfigurationElement::ValueType retval = Widget::find(path);
        if (retval.has_value())
            return retval;

        retval = dagbase::findEndpoint(path, "title", _title);
        if (retval.has_value())
            return retval;

        return {};
    }

    std::string Window::featuresName(Window::Features feature)
    {
        std::string str;

        BIT_NAME(feature, TITLE_BIT, str)
        BIT_NAME(feature, MINIMISE_BIT, str)
        BIT_NAME(feature, MAXIMISE_BIT, str)
        BIT_NAME(feature, CLOSE_BIT, str)
        BIT_NAME(feature, BORDER_BIT, str)
        BIT_NAME(feature, RESIZEABLE_BIT, str)
        BIT_NAME(feature, MOVEABLE_BIT, str)

        if (!str.empty() && str.back() == ' ')
        {
            str = str.substr(0,str.length()-1);
        }

        return std::move(str);
    }

    Window::Features Window::parseFeatures(const std::string& str)
    {
        Features mask{0x0};

        TEST_BIT(TITLE_BIT, str, mask)
        TEST_BIT(MINIMISE_BIT, str, mask)
        TEST_BIT(MAXIMISE_BIT, str, mask)
        TEST_BIT(CLOSE_BIT, str, mask)
        TEST_BIT(BORDER_BIT, str, mask)
        TEST_BIT(RESIZEABLE_BIT, str, mask)
        TEST_BIT(MOVEABLE_BIT, str, mask)

        return mask;
    }

    std::string Window::statusName(Window::Status status)
    {
        std::string str;

        BIT_NAME(status, VISIBLE_BIT, str)
        BIT_NAME(status, MINIMISED_BIT, str)
        BIT_NAME(status, MAXIMISED_BIT, str)

        if (!str.empty() && str.back() == ' ')
        {
            str = str.substr(0,str.length()-1);
        }

        return std::move(str);
    }

    Window::Status Window::parseStatus(const std::string &str)
    {
        Status status{0x0};

        TEST_BIT(VISIBLE_BIT, str, status)
        TEST_BIT(MINIMISED_BIT, str, status)
        TEST_BIT(MAXIMISED_BIT, str, status)

        return status;
    }
}
