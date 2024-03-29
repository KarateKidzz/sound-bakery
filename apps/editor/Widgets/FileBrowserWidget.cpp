#include "FileBrowserWidget.h"

#include "App/App.h"
#include "IconsFontAwesome6.h"
#include "IconsFontaudio.h"
#include "Managers/ProjectManager.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "sound_bakery/editor/editor_defines.h"
#include "sound_bakery/node/container/sound_container.h"

void FileBrowserWidget::UnselectItem() noexcept
{
    m_selectedItemID = std::numeric_limits<uint32_t>::max();
}

ButtonState FileBrowserWidget::DrawWideButton(
    bool selected, uint32_t hovered_color, uint32_t active_color) const noexcept
{
    const auto& style = ImGui::GetStyle();
    auto cursor_pos   = ImGui::GetCursorPos();
    ImGui::Dummy({ImGui::GetContentRegionMax().x,
                  ImGui::GetTextLineHeight() + style.FramePadding.y * 2.0f});

    const auto item_min = ImGui::GetItemRectMin();
    const auto item_max = ImGui::GetItemRectMax();

    cursor_pos.y += style.FramePadding.y;
    ImGui::SetCursorPos(cursor_pos);

    auto draw_list = ImGui::GetWindowDrawList();

    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            draw_list->AddRectFilled(item_min, item_max, active_color);
            return ButtonState::CLICKED;
        }

        draw_list->AddRectFilled(item_min, item_max, hovered_color);
        return ButtonState::HOVERED;
    }

    if (selected)
        draw_list->AddRectFilled(item_min, item_max, active_color);

    return ButtonState::NONE;
}

void FileBrowserWidget::ShowItemContextMenu(
    const std::filesystem::path& path) const noexcept
{
    if (ImGui::Button("Rename"))
    {
        ImGui::OpenPopup("RenamePopup");
    }

    if (ImGui::BeginPopup("RenamePopup"))
    {
        static char name[255] = "";
        ImGui::Text("Edit name:");
        ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));

        if (ImGui::Button("Confirm"))
        {
            std::filesystem::path prevExtension = path.extension();
            std::filesystem::path newPath(name);

            if (newPath.extension().empty())
            {
                newPath /= prevExtension;
            }

            const std::filesystem::path fullOldPath =
                std::filesystem::absolute(m_currentDirectory / path);
            const std::filesystem::path fullNewPath =
                std::filesystem::absolute(m_currentDirectory / newPath);

            if (std::filesystem::exists(fullOldPath))
            {
                std::filesystem::rename(fullOldPath, fullNewPath);

                std::memset(&name, 0, IM_ARRAYSIZE(name));
            }
        }

        ImGui::EndPopup();
    }

    ImGui::Separator();
}

void FileBrowserWidget::ShowNavMenu() noexcept
{
    if (ImGui::ArrowButton("Back", ImGuiDir_Left))
    {
        // Don't go higher than the top most directory
        if (m_currentDirectory != m_topDir)
        {
            m_currentDirectory = m_currentDirectory.parent_path();
            UnselectItem();
            m_selectedFileString.clear();
        }
    }

    ImGui::SameLine();
    std::string display_text = m_currentDirectory.string();
    display_text.erase(
        0, m_topDir.string().length() - m_topDir.filename().string().length());

    static std::string match       = "/";
    static std::string replacement = " > ";

    size_t pos = display_text.find(match);
    while (pos != std::string::npos)
    {
        display_text.replace(pos, match.length(), replacement);
        pos = display_text.find(match, pos + replacement.length());
    }

    ImGui::TextUnformatted(display_text.c_str());
}

void FileBrowserWidget::ShowDirectoryBrowserList() noexcept
{
    static const auto folder_color = IM_COL32(255, 255, 0, 255);
    static const auto file_color   = IM_COL32(0, 189, 255, 255);
    static const auto icon_size    = ImGui::GetTextLineHeight();

    auto& style = ImGui::GetStyle();
    const auto btn_hovered_color =
        ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBgHovered]);
    const auto btn_active_color =
        ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBgActive]);

    if (ImGui::BeginTable("Dir Browser Table", 1,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg))
    {
        uint32_t id = 0;
        if (m_currentDirectory.is_absolute())
        {
            for (auto& p :
                 std::filesystem::directory_iterator(m_currentDirectory))
            {
                ImGui::TableNextColumn();

                const bool selected  = (id == m_selectedItemID);
                const auto btn_state = DrawWideButton(
                    selected, btn_hovered_color, btn_active_color);

                if (selected && ImGui::IsItemHovered() &&
                    ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                {
                    m_selectedFileString = p.path().string();
                }

                ImGui::PushID(p.path().filename().string().c_str());

                if (ImGui::BeginPopupContextItem("File Context Menu"))
                {
                    if (selected)
                    {
                        ShowItemContextMenu(p.path().filename());
                    }
                    else
                    {
                        UnselectItem();
                    }

                    ImGui::EndPopup();
                }

                if (btn_state == ButtonState::CLICKED)
                {
                    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        if (p.is_directory())
                        {
                            m_currentDirectory = p.path();
                            UnselectItem();
                        }
                    }
                    else
                    {
                        m_selectedItemID = id;
                    }
                }

                std::string filename = p.path().filename().string();

                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

                if (p.is_regular_file())
                {
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    ImGui::Text("%s %s", ICON_FAD_WAVEFORM, filename.c_str());
                    ImGui::PopFont();
                }
                else if (p.is_directory())
                {
                    ImGui::Text("%s %s", ICON_FA_FOLDER, filename.c_str());
                }
                else
                {
                    ImGui::Text("%s %s", ICON_FA_FILE, filename.c_str());
                }

                ImGui::PopFont();

                if (ImGui::BeginPopupContextItem("item context menu"))
                {
                    // m_selectedItemID = id;

                    ShowItemContextMenu(p);
                    ImGui::EndPopup();
                }

                if (ImGui::BeginDragDropSource(
                        ImGuiDragDropFlags_SourceAllowNullID))
                {
                    m_draggingFile = p.path().string();

                    // Set payload to carry the index of our item (could be
                    // anything)
                    ImGui::SetDragDropPayload(
                        SB::Editor::PayloadSound.c_str(),
                        m_draggingFile.c_str(),
                        (m_draggingFile.size() + 1) * sizeof(char),
                        ImGuiCond_Once);

                    ImGui::TextUnformatted(m_draggingFile.c_str());

                    ImGui::EndDragDropSource();
                }

                ImGui::PopID();
                ++id;
            }
        }

        ImGui::EndTable();
    }
}

void FileBrowserWidget::Render()
{
    ShowNavMenu();
    ImGui::Separator();

    if (ImGui::BeginTable("ImGroot Main", 1,
                          ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
    {
        ImGui::TableSetupColumn("Files");

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);

        ShowDirectoryBrowserList();

        ImGui::EndTable();
    }

    if (m_selectedFileString != m_selectedFile)
    {
        m_selectedFile = m_selectedFileString;

        if (ProjectManager* manager = GetApp()->GetProjectManager())
        {
            // manager->GetPreviewSoundContainer()->setSound(m_selectedFile);
            // manager->GetSelection().SelectAudioFile(m_selectedFile);
        }
    }
}
