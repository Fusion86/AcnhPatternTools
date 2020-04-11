#include "DesignPatternsView.hpp"

#include <wx/wrapsizer.h>

IDesignPatternProxy::~IDesignPatternProxy() {}

DesignPatternsView::DesignPatternsView(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    //
    // Column 0 - Listboxes
    //

    wxStaticText* lblDesignPatterns = new wxStaticText(this, wxID_ANY, "Design Patterns");
    wxStaticText* lblProDesignPatterns = new wxStaticText(this, wxID_ANY, "Pro Design Patterns");

    lstDesignPatterns = new wxListBox(this, ID_lstDesignPatterns, wxDefaultPosition, wxDefaultSize);
    lstProDesignPatterns =
        new wxListBox(this, ID_lstProDesignPatterns, wxDefaultPosition, wxDefaultSize);

    wxFlexGridSizer* col0 = new wxFlexGridSizer(1, wxSize(10, 5));
    col0->Add(lblDesignPatterns);
    col0->Add(lstDesignPatterns, 1, wxEXPAND);
    col0->Add(lblProDesignPatterns);
    col0->Add(lstProDesignPatterns, 1, wxEXPAND);

    col0->AddGrowableRow(1);
    col0->AddGrowableRow(3);

    //
    // Column 1 - Info panel with name, creator, etc
    //

    wxWrapSizer* hboxPalette = new wxWrapSizer(wxHORIZONTAL);

    for (int i = 0; i < 16; i++) {
        paletteButtons[i] =
            new ColorRectButton(this, wxID_ANY, wxColour(), wxDefaultPosition, wxSize(40, 40));
        hboxPalette->Add(paletteButtons[i]);
    }

    wxStaticText* lblName = new wxStaticText(this, wxID_ANY, "Name");
    wxStaticText* lblCreator = new wxStaticText(this, wxID_ANY, "Creator");
    wxStaticText* lblCreatorId = new wxStaticText(this, wxID_ANY, "Creator ID");
    wxStaticText* lblIsland = new wxStaticText(this, wxID_ANY, "Island");
    wxStaticText* lblIslandId = new wxStaticText(this, wxID_ANY, "Island ID");
    wxStaticText* lblPalette = new wxStaticText(this, wxID_ANY, "Palette");

    txtPatternName = new wxTextCtrl(this, ID_txtPatternName);
    txtPatternCreator = new wxTextCtrl(this, ID_txtPatternCreator);
    txtPatternCreatorId = new wxTextCtrl(this, ID_txtPatternCreatorId);
    txtPatternIsland = new wxTextCtrl(this, ID_txtPatternIsland);
    txtPatternIslandId = new wxTextCtrl(this, ID_txtPatternIslandId);

    wxFlexGridSizer* col1 = new wxFlexGridSizer(2, wxSize(10, 5));
    col1->Add(lblName, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(txtPatternName, 1, wxEXPAND);
    col1->Add(lblCreator, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(txtPatternCreator, 1, wxEXPAND);
    col1->Add(lblCreatorId, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(txtPatternCreatorId, 1, wxEXPAND);
    col1->Add(lblIsland, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(txtPatternIsland, 1, wxEXPAND);
    col1->Add(lblIslandId, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(txtPatternIslandId, 1, wxEXPAND);
    col1->Add(lblPalette, 0, wxALIGN_CENTER_VERTICAL);
    col1->Add(hboxPalette, 1, wxEXPAND);

    col1->AddGrowableCol(1);

    //
    // Column 2- Pattern preview image
    //

    wxBitmap bmpPattern = wxBitmap();
    bmpPattern.Create(wxSize(320, 320));
    bmpPatternCtrl = new wxGenericStaticBitmap(this, wxID_ANY, bmpPattern);

    wxButton* btnImportPattern = new wxButton(this, ID_btnImportPattern, "Import image");
    wxButton* btnPreviewMultiTile =
        new wxButton(this, ID_btnPreviewMultiTile, "View multi-tile image");

    wxBoxSizer* col2 = new wxBoxSizer(wxVERTICAL);
    col2->Add(bmpPatternCtrl);
    col2->AddSpacer(5);
    col2->Add(btnImportPattern, 0, wxEXPAND);
    col2->AddSpacer(5);
    col2->Add(btnPreviewMultiTile, 0, wxEXPAND);

    //
    // Connect all the things
    //

    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(col0, 0, wxEXPAND | wxALL, 10);
    hbox->Add(col1, 1, wxEXPAND | wxALL | wxALIGN_LEFT, 10);
    hbox->Add(col2, 0, wxALL, 10);

    Bind(EVT_DATA_CHANGED, &DesignPatternsView::onDataChanged, this);
    Bind(wxEVT_LISTBOX, &DesignPatternsView::onSelectionChanged, this);
    Bind(wxEVT_TEXT, &DesignPatternsView::onPatternNameChanged, this, ID_txtPatternName);
    Bind(wxEVT_BUTTON, &DesignPatternsView::onImportImage, this, ID_btnImportPattern);

    SetSizer(hbox);
}

void DesignPatternsView::onDataChanged(wxCommandEvent& event) {
    wxArrayString patternNames;
    wxArrayString proPatternNames;

    designPatterns.clear();
    proDesignPatterns.clear();

    for (int i = 0; i < 50; i++) {
        designPatterns.push_back(
            DesignPatternProxy(AppState->savedata->main.designPatterns[i], i, lstDesignPatterns));
        proDesignPatterns.push_back(ProDesignPatternProxy(
            AppState->savedata->main.proDesignPatterns[i], i, lstProDesignPatterns));

        patternNames.Add(designPatterns[i].getName());
        proPatternNames.Add(proDesignPatterns[i].getName());
    }

    lstDesignPatterns->Set(patternNames);
    lstProDesignPatterns->Set(patternNames);
}

void DesignPatternsView::onSelectionChanged(wxCommandEvent& event) {
    int idx = event.GetSelection();
    switch (event.GetId()) {
        case ID_lstDesignPatterns: {
            auto idx = lstDesignPatterns->GetSelection();
            if (idx == wxNOT_FOUND) break;
            setSelectedPattern(&designPatterns[idx]);
        } break;

        case ID_lstProDesignPatterns: {
            auto patternIdx = lstProDesignPatterns->GetSelection();
            if (patternIdx == wxNOT_FOUND) break;
            setSelectedPattern(&proDesignPatterns[idx]);
        } break;

        default:
            // log warning, this shouldn't happen!
            break;
    }
}

void DesignPatternsView::onPatternNameChanged(wxCommandEvent& event) {
    if (selectedDesignPattern != nullptr) {
        selectedDesignPattern->setName(txtPatternName->GetValue().ToStdString());
    }
}

void DesignPatternsView::onImportImage(wxCommandEvent& event) {
    if (selectedDesignPattern == nullptr) {
        wxMessageBox("Please select a design pattern to be used as the destination.",
                     "No design pattern selected.");
    } else {
        if (selectedDesignPattern->isProPattern()) {
            wxMessageBox("The image import functionality currently only works for normal patterns.",
                         "Not supported");
            return;
        }

        wxFileDialog dialog(this, "Select source image");
        if (dialog.ShowModal() == wxID_OK) {
            wxImage img;
            if (not img.LoadFile(dialog.GetPath())) {
                wxMessageBox("Couldn't load image!", "Couldn't load image");
                return;
            }

            if (img.GetWidth() % 32 != 0 || img.GetHeight() % 32 != 0) {
                wxMessageBox("The image needs to be 32x32, or a multiple of 32x32 (aspect ratio "
                             "doesn't matter).",
                             "Invalid image size");
                return;
            }

            // TODO: Place this in a function somewhere
            auto getOrInsertColor = [](std::vector<AcnhColor>& palette,
                                       const AcnhColor color) -> int {
                // Return existing color, if possible
                for (size_t i = 0; i < palette.size(); i++) {
                    if (palette[i] == color) return i;
                }

                // Insert new color in palette, if there's space
                if (palette.size() < 15) {
                    palette.push_back(color);
                    return palette.size() - 1;
                }

                // No space available for more colors :(
                return -1;
            };

            size_t rows = img.GetHeight() / 32;
            size_t cols = img.GetWidth() / 32;
            size_t designOffset = selectedDesignPattern->getIndex();
            std::vector<AcnhColor> palette;

            wxMessageDialog dialog(
                this,
                fmt::format("This image will overwrite {} patterns, starting at {} (zero-indexed).",
                            rows * cols, designOffset),
                "Continue?", wxOK | wxCANCEL | wxCENTER);
            if (dialog.ShowModal() != wxID_OK) {
                return;
            }

            for (size_t row = 0; row < rows; row++) {
                for (size_t col = 0; col < cols; col++) {
                    size_t part =
                        row * rows + col; // 1 part = 32x32, so 128x128 images have 16 parts
                    size_t y_off = row * 32;
                    size_t x_off = col * 32;

                    for (int y = 0; y < 32; y++) {
                        for (int x = 0; x < 32; x++) {
                            AcnhColor color{img.GetRed(x_off + x, y_off + y),
                                            img.GetBlue(x_off + x, y_off + y),
                                            img.GetGreen(x_off + x, y_off + y)};

                            int idx = 0xF; // Transparent by default
                            if (not img.HasAlpha() || img.GetAlpha(x_off + x, y_off + y) != 0) {
                                // Get color palette index if pixel is NOT transparent
                                idx = getOrInsertColor(palette, color);
                            }
                            AppState->savedata->main.designPatterns[designOffset + part].setPixel(
                                x, y, idx);
                        }
                    }
                }
            }

            // Update palette in pattern
            // TODO: Write non-shit code (build AcnhColor[] once and then set for all)
            for (size_t j = 0; j < rows * cols; j++) {
                for (size_t i = 0; i < 15; i++) {
                    AcnhColor color{};
                    if (palette.size() > i) color = palette[i];
                    AppState->savedata->main.designPatterns[designOffset + j].palette[i] = color;
                }
            }

            // TODO: Update pattern name
        }
    }
}

void DesignPatternsView::setSelectedPattern(IDesignPatternProxy* pattern) {
    selectedDesignPattern = pattern;
    const auto rgb = pattern->getRgbData();
    const auto alpha = pattern->getAlphaData();
    const auto res = pattern->getResolution();

    wxImage img = wxImage(res, res, rgb.get(), alpha.get(), true);
    img.Rescale(320, 320);
    wxBitmap bmp = wxBitmap(img);
    bmpPatternCtrl->SetBitmap(bmp);

    txtPatternName->ChangeValue(pattern->getName());
    txtPatternCreator->ChangeValue(pattern->getCreatorName());
    txtPatternCreatorId->ChangeValue(pattern->getCreatorId());
    txtPatternIsland->ChangeValue(pattern->getIslandName());
    txtPatternIslandId->ChangeValue(pattern->getIslandId());

    const auto& palette = pattern->getPalette();
    for (size_t i = 0; i < palette.size(); i++) {
        paletteButtons[i]->setColor(palette[i]);
    }

    if (pattern->isProPattern()) {
        lstDesignPatterns->DeselectAll();
    } else {
        lstProDesignPatterns->DeselectAll();
    }
}
