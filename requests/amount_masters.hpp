#ifndef COURSEWORK_AMOUNT_MASTERS_HPP
#define COURSEWORK_AMOUNT_MASTERS_HPP

#include "../imwindows/imwindows.hpp"
#include "../nlohmann/json/json.hpp"
#include "../plumbing/master.hpp"
#include "utils.hpp"

namespace Requests {
    struct AmountMasters {
        static const char *getMenuName() {
            return "Кількість майстрів";
        }

        static void render() {
            if (ImGui::Button("Додати майстра"))
                ImGui::OpenPopup("Додати майстра");
            ImGui::SameLine();
            ImGui::Text("Всього майстрів: %zu", GetAllMasters().size());
            if (ImGui::BeginPopup("Додати майстра")) {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1);
                renderAddMaster();
                ImGui::PopStyleVar(1);
                ImGui::EndPopup();
            }
            if (ImGui::BeginTable("tablemasters", 8, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("# віділення");
                ImGui::TableSetupColumn("ПІБ");
                ImGui::TableSetupColumn("Адреса");
                ImGui::TableSetupColumn("Номер телефону");
                ImGui::TableSetupColumn("Стаж(роки)");
                ImGui::TableSetupColumn("# бригади");
                ImGui::TableSetupColumn("Стан");
                ImGui::TableSetupColumn("##delete", ImGuiTableColumnFlags_WidthFixed, 20.f);

                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                size_t remove_item_index{0};
                for (size_t i{0}; const auto &master : GetAllMasters()) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", master["departament_number"].get<int>());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", master["full_name"].get<std::string>().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", master["address"].get<std::string>().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", master["phone_number"].get<std::string>().c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", master["experience"].get<int>());
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", master["brigade_number"].get<int>());
                    ImGui::TableNextColumn();
                    switch (master["status"].get<Master::Status>()) {
                        using
                        enum Master::Status;
                        case kFree:
                            ImGui::Text("Вільний");
                            break;
                        case kInTheWay:
                            ImGui::Text("В дорозі");
                            break;
                        case kBusy:
                            ImGui::Text("Зайнятий");
                            break;
                    }
                    ImGui::TableNextColumn();
                    if (ImGui::Button(("x##" + std::to_string(i++)).c_str()))
                        remove_item_index = i;
                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal))
                        ImGui::SetTooltip("Видалити запис");
                }
                if (remove_item_index != 0)
                    GetAllMasters().erase(remove_item_index - 1);
                ImGui::EndTable();
            }
        }

        static void renderAddMaster() {
            static struct {
                int departament_number;
                char full_name[256];
                char address[256];
                char phone_number[15];
                int experience;
                int brigade_number;
            } input_master{};
            ImGui::InputInt("Номер відділення##input", &input_master.departament_number);
            ImGui::InputText("ПІБ##input", input_master.full_name, sizeof(input_master.full_name));
            ImGui::InputText("Адреса##input", input_master.address, sizeof(input_master.address));
            ImGui::InputText("Номер телефону##input", input_master.phone_number, sizeof(input_master.phone_number));
            ImGui::InputInt("Стаж(роки)##input", &input_master.experience);
            ImGui::InputInt("Номер бригади##input", &input_master.brigade_number);
            if (ImGui::Button("Додати майстра##input", {-1, 30})) {
                if (strlen(input_master.full_name) == 0 || strlen(input_master.address) == 0) {
                    ImGui::OpenPopup("Помилка");
                } else {
                    Master temp
                            {input_master.departament_number, input_master.full_name, input_master.address,
                             input_master.phone_number,
                             input_master.experience, input_master.brigade_number};
                    GetAllMasters().push_back(temp);
                    ZeroMemory(&input_master, sizeof(input_master));
                    SortMasters();
                }
            }
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), 0, {0.5f, 0.5f});
            if (ImGui::BeginPopupModal("Помилка", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                if (strlen(input_master.full_name) == 0)
                    ImGui::Text("Заповніть поле з ПІБ");
                if (strlen(input_master.address) == 0)
                    ImGui::Text("Заповніть поле з адресою");
                if (ImGui::Button("Закрити##input", {-1, 0}))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
        }
    };
}

#endif //COURSEWORK_AMOUNT_MASTERS_HPP
