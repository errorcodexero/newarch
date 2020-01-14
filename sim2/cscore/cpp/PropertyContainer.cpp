/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2019 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "PropertyContainer.h"

#include <wpi/Logger.h>
#include <wpi/SmallString.h>
#include <wpi/SmallVector.h>
#include <wpi/json.h>

using namespace cs;

int PropertyContainer::GetPropertyIndex(const wpi::Twine& name) const {
  // We can't fail, so instead we create a new index if caching fails.
  CS_Status status = 0;
  if (!m_properties_cached) CacheProperties(&status);
  std::scoped_lock lock(m_mutex);
  wpi::SmallVector<char, 64> nameBuf;
  int& ndx = m_properties[name.toStringRef(nameBuf)];
  if (ndx == 0) {
    // create a new index
    ndx = m_propertyData.size() + 1;
    m_propertyData.emplace_back(CreateEmptyProperty(name));
  }
  return ndx;
}

wpi::ArrayRef<int> PropertyContainer::EnumerateProperties(
    wpi::SmallVectorImpl<int>& vec, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status))
    return wpi::ArrayRef<int>{};
  std::scoped_lock lock(m_mutex);
  for (int i = 0; i < static_cast<int>(m_propertyData.size()); ++i) {
    if (m_propertyData[i]) vec.push_back(i + 1);
  }
  return vec;
}

CS_PropertyKind PropertyContainer::GetPropertyKind(int property) const {
  CS_Status status = 0;
  if (!m_properties_cached && !CacheProperties(&status)) return CS_PROP_NONE;
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) return CS_PROP_NONE;
  return prop->propKind;
}

wpi::StringRef PropertyContainer::GetPropertyName(
    int property, wpi::SmallVectorImpl<char>& buf, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return wpi::StringRef{};
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return wpi::StringRef{};
  }
  // safe to not copy because we never modify it after caching
  return prop->name;
}

int PropertyContainer::GetProperty(int property, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return 0;
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return 0;
  }
  if ((prop->propKind & (CS_PROP_BOOLEAN | CS_PROP_INTEGER | CS_PROP_ENUM)) ==
      0) {
    *status = CS_WRONG_PROPERTY_TYPE;
    return 0;
  }
  return prop->value;
}

void PropertyContainer::SetProperty(int property, int value,
                                    CS_Status* status) {
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return;
  }

  // Guess it's integer if we've set before get
  if (prop->propKind == CS_PROP_NONE) prop->propKind = CS_PROP_INTEGER;

  if ((prop->propKind & (CS_PROP_BOOLEAN | CS_PROP_INTEGER | CS_PROP_ENUM)) ==
      0) {
    *status = CS_WRONG_PROPERTY_TYPE;
    return;
  }

  UpdatePropertyValue(property, false, value, wpi::Twine{});
}

int PropertyContainer::GetPropertyMin(int property, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return 0;
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return 0;
  }
  return prop->minimum;
}

int PropertyContainer::GetPropertyMax(int property, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return 0;
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return 0;
  }
  return prop->maximum;
}

int PropertyContainer::GetPropertyStep(int property, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return 0;
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return 0;
  }
  return prop->step;
}

int PropertyContainer::GetPropertyDefault(int property,
                                          CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return 0;
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return 0;
  }
  return prop->defaultValue;
}

wpi::StringRef PropertyContainer::GetStringProperty(
    int property, wpi::SmallVectorImpl<char>& buf, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status)) return wpi::StringRef{};
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return wpi::StringRef{};
  }
  if (prop->propKind != CS_PROP_STRING) {
    *status = CS_WRONG_PROPERTY_TYPE;
    return wpi::StringRef{};
  }
  buf.clear();
  buf.append(prop->valueStr.begin(), prop->valueStr.end());
  return wpi::StringRef(buf.data(), buf.size());
}

void PropertyContainer::SetStringProperty(int property, const wpi::Twine& value,
                                          CS_Status* status) {
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return;
  }

  // Guess it's string if we've set before get
  if (prop->propKind == CS_PROP_NONE) prop->propKind = CS_PROP_STRING;

  if (prop->propKind != CS_PROP_STRING) {
    *status = CS_WRONG_PROPERTY_TYPE;
    return;
  }

  UpdatePropertyValue(property, true, 0, value);
}

std::vector<std::string> PropertyContainer::GetEnumPropertyChoices(
    int property, CS_Status* status) const {
  if (!m_properties_cached && !CacheProperties(status))
    return std::vector<std::string>{};
  std::scoped_lock lock(m_mutex);
  auto prop = GetProperty(property);
  if (!prop) {
    *status = CS_INVALID_PROPERTY;
    return std::vector<std::string>{};
  }
  if (prop->propKind != CS_PROP_ENUM) {
    *status = CS_WRONG_PROPERTY_TYPE;
    return std::vector<std::string>{};
  }
  return prop->enumChoices;
}

std::unique_ptr<PropertyImpl> PropertyContainer::CreateEmptyProperty(
    const wpi::Twine& name) const {
  return std::make_unique<PropertyImpl>(name);
}

bool PropertyContainer::CacheProperties(CS_Status* status) const {
  // Doesn't need to do anything.
  m_properties_cached = true;
  return true;
}

bool PropertyContainer::SetPropertiesJson(const wpi::json& config,
                                          wpi::Logger& logger,
                                          wpi::StringRef logName,
                                          CS_Status* status) {
  for (auto&& prop : config) {
    std::string name;
    try {
      name = prop.at("name").get<std::string>();
    } catch (const wpi::json::exception& e) {
      WPI_WARNING(logger,
                  logName << ": SetConfigJson: could not read property name: "
                          << e.what());
      continue;
    }
    int n = GetPropertyIndex(name);
    try {
      auto& v = prop.at("value");
      if (v.is_string()) {
        std::string val = v.get<std::string>();
        WPI_INFO(logger, logName << ": SetConfigJson: setting property '"
                                 << name << "' to '" << val << '\'');
        SetStringProperty(n, val, status);
      } else if (v.is_boolean()) {
        bool val = v.get<bool>();
        WPI_INFO(logger, logName << ": SetConfigJson: setting property '"
                                 << name << "' to " << val);
        SetProperty(n, val, status);
      } else {
        int val = v.get<int>();
        WPI_INFO(logger, logName << ": SetConfigJson: setting property '"
                                 << name << "' to " << val);
        SetProperty(n, val, status);
      }
    } catch (const wpi::json::exception& e) {
      WPI_WARNING(logger,
                  logName << ": SetConfigJson: could not read property value: "
                          << e.what());
      continue;
    }
  }

  return true;
}

wpi::json PropertyContainer::GetPropertiesJsonObject(CS_Status* status) {
  wpi::json j;
  wpi::SmallVector<int, 32> propVec;
  for (int p : EnumerateProperties(propVec, status)) {
    wpi::json prop;
    wpi::SmallString<128> strBuf;
    prop.emplace("name", GetPropertyName(p, strBuf, status));
    switch (GetPropertyKind(p)) {
      case CS_PROP_BOOLEAN:
        prop.emplace("value", static_cast<bool>(GetProperty(p, status)));
        break;
      case CS_PROP_INTEGER:
      case CS_PROP_ENUM:
        prop.emplace("value", GetProperty(p, status));
        break;
      case CS_PROP_STRING:
        prop.emplace("value", GetStringProperty(p, strBuf, status));
        break;
      default:
        continue;
    }
    j.emplace_back(prop);
  }

  return j;
}
