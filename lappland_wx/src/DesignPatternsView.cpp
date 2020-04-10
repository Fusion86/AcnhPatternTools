#include "DesignPatternsView.hpp"

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

    wxStaticText* lblName = new wxStaticText(this, wxID_ANY, "Name");
    wxStaticText* lblCreator = new wxStaticText(this, wxID_ANY, "Creator");
    wxStaticText* lblCreatorId = new wxStaticText(this, wxID_ANY, "Creator ID");
    wxStaticText* lblIsland = new wxStaticText(this, wxID_ANY, "Island");
    wxStaticText* lblIslandId = new wxStaticText(this, wxID_ANY, "Island ID");

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

    col1->AddGrowableCol(1);

    //
    // Column 2- Pattern preview image
    //

    wxBitmap bmpPattern = wxBitmap();
    bmpPattern.Create(wxSize(320, 320));
    bmpPatternCtrl = new wxGenericStaticBitmap(this, wxID_ANY, bmpPattern);

    wxBoxSizer* col2 = new wxBoxSizer(wxHORIZONTAL);
    col2->Add(bmpPatternCtrl);

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

    if (pattern->isProPattern()) {
        lstDesignPatterns->DeselectAll();
    } else {
        lstProDesignPatterns->DeselectAll();
    }
}
