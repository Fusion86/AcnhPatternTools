#include "ui/LapplandApplication.hpp"

namespace ui {
    void LapplandApplication::OnLoad() {
        this->mainLayout = MainLayout::New();
        this->LoadLayout(this->mainLayout);

        this->SetOnInput([&](u64 Down, u64 Up, u64 Held, pu::ui::Touch Pos) {
            if (Down & KEY_X) // If X is pressed, start with our dialog questions!
            {
                // int opt = this->CreateShowDialog("Question", "Do you like apples?",
                //                                  {"Yes!", "No...", "Cancel"},
                //                                  true); // (using latest option as cancel option)
                // if ((opt == -1) ||
                //     (opt == -2)) // -1 and -2 are similar, but if the user cancels manually -1 is
                //                  // set, other types or cancel should be -2.
                // {
                //     this->CreateShowDialog("Cancel", "Last question was canceled.", {"Ok"},
                //                            true); // If we will ignore the option, it doesn't matter
                //                                   // if this is true or false
                // } else {
                //     switch (opt) {
                //     case 0: // "Yes" was selected
                //         this->CreateShowDialog("Answer", "Really? I like apples too!", {"Ok"},
                //                                true); // Same here ^
                //         break;
                //     case 1: // "No" was selected
                //         this->CreateShowDialog("Answer", "Oh, bad news then...", {"OK"},
                //                                true); // And here ^
                //         break;
                //     }
                // }
            } else if (Down & KEY_PLUS) // If + is pressed, exit application
            {
                this->Close();
            }
        });
    }
} // namespace ui
