#ifndef TENTRIS_VERSIONSTRINGS_HPP
#define TENTRIS_VERSIONSTRINGS_HPP

#include <tentris/tentris_version.hpp>
#include <Dice/hypertrie/hypertrie_version.hpp>

inline const std::string tentris_version_string = std::string{} + Dice::tentris::name + " " + Dice::tentris::version;

inline const std::string hypertrie_version_string = std::string{} + Dice::hypertrie::name + " " + Dice::hypertrie::version;

#endif //TENTRIS_VERSIONSTRINGS_HPP
