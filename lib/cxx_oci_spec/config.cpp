#include "cxx_oci_spec/config.hpp"

#include <black_desk/cpplib/error.hpp>

namespace cxx_oci_spec
{

auto Config::from(const std::filesystem::path &file) -> std::shared_ptr<Config>
{
        std::ifstream config_fs;
        config_fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        config_fs.open(file);

        nlohmann::json config_json;
        config_json = nlohmann::json::parse(config_fs);
        return from(config_json);
}

auto Config::from(const nlohmann::json &json) -> std::shared_ptr<Config>
{
        try {
                auto ret = std::make_shared<Config>();
                json.get_to(*ret);
                return ret;
        } catch (const nlohmann::json::exception &e) {
                NESTED_EXCEPTION(
                        "Parsing config json failed, check missing required fields.");
        }
}

} // namespace cxx_oci_spec
