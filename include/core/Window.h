//
// Created by tony on 19/08/24.
//

#pragma once

#include "config/Export.h"

#include "core/Widget.h"

#include <cstdint>

namespace dagui
{
    class DAGUI_API Window : public Widget
    {
    public:
        //! Features of the window such as presence of certain items
        enum Features : std::uint32_t
        {
            NO_FEATURES = 0,
            //! Title bar is present
            TITLE_BIT = 1 << 0,
            //! Minimise button is present
            MINIMISE_BIT = 1 << 1,
            //! Maximise button is present
            MAXIMISE_BIT = 1 << 2,
            //! Close button is present
            CLOSE_BIT = 1 << 3,
            //! Border is present
            BORDER_BIT = 1 << 4,
            //! Window can be resized
            RESIZEABLE_BIT = 1 << 5,
            //! Window can be moved
            MOVEABLE_BIT = 1 << 6,
            //! Features that are available by default
            DEFAULT_FEATURES = TITLE_BIT | MINIMISE_BIT | MAXIMISE_BIT | CLOSE_BIT | BORDER_BIT | RESIZEABLE_BIT | MOVEABLE_BIT
        };

        //! Current status of window
        enum Status : std::uint32_t
        {
            //! Window is on the screen
            VISIBLE_BIT = 1 << 0,
            //! Window is minimised(collapsed)
            MINIMISED_BIT = 1 << 1,
            //! Window is taking up full parent region
            MAXIMISED_BIT = 1 << 2,
        };
    public:
        //! \constructor
        //! \param parent : Widget The parent of this Window, can be nullptr if this is the root window.
        explicit Window(Widget* parent);

        bool areFeaturesAvailable(Features features) const
        {
            return (_features & features) == features;
        }

        static std::string featuresName(Features feature);

        static Features parseFeatures(const std::string& str);

        static std::string statusName(Status status);

        static Status parseStatus(const std::string& str);
    private:
        Features _features{ DEFAULT_FEATURES };
        Status _status{ 0x0 };
    };
}
