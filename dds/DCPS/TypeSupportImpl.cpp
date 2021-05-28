/*
 *
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#include "DCPS/DdsDcps_pch.h" //Only the _pch include should start with DCPS/

#include "TypeSupportImpl.h"

#include "Registered_Data_Types.h"
#include "XTypes/TypeLookupService.h"

OPENDDS_BEGIN_VERSIONED_NAMESPACE_DECL

namespace OpenDDS {
namespace DCPS {

TypeSupportImpl::~TypeSupportImpl()
{}

DDS::ReturnCode_t
TypeSupportImpl::register_type(DDS::DomainParticipant_ptr participant,
                               const char* type_name)
{
  const char* const type =
    (!type_name || !type_name[0]) ? default_type_name() : type_name;
  return Registered_Data_Types->register_type(participant, type, this);
}

DDS::ReturnCode_t
TypeSupportImpl::unregister_type(DDS::DomainParticipant_ptr participant,
    const char* type_name)
{
  if (type_name == 0 || type_name[0] == '\0') {
    return DDS::RETCODE_BAD_PARAMETER;
  } else {
    return Registered_Data_Types->unregister_type(participant, type_name, this);
  }
}

char*
TypeSupportImpl::get_type_name()
{
  CORBA::String_var type = default_type_name();
  return type._retn();
}

void TypeSupportImpl::to_type_info_i(XTypes::TypeIdentifierWithDependencies& ti_with_deps,
                                     const XTypes::TypeIdentifier& ti,
                                     const XTypes::TypeMap& type_map) const
{
  const TypeMap::const_iterator pos = type_map.find(ti);

  if (pos == type_map.end()) {
    std::string ek = ti.kind() == XTypes::EK_MINIMAL ? "minimal" : "complete";
    ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TypeSupportImpl::to_type_info_i, ")
               ACE_TEXT("%C TypeIdentifier of topic type not found in local type map.\n"), ek.c_str()));
    ti_with_deps.typeid_with_size.type_id = TypeIdentifier();
    ti_with_deps.typeid_with_size.typeobject_serialized_size = 0;
  } else {
    ti_with_deps.typeid_with_size.type_id = ti;
    const TypeObject& to = pos->second;
    const size_t sz = serialized_size(get_typeobject_encoding(), to);
    ti_with_deps.typeid_with_size.typeobject_serialized_size = static_cast<ACE_CDR::ULong>(sz);
  }
  ti_with_deps.dependent_typeid_count = -1;
}

void TypeSupportImpl::to_type_info(XTypes::TypeInformation& type_info) const
{
  to_type_info_i(type_info.minimal, getMinimalTypeIdentifier(), getMinimalTypeMap());
  to_type_info_i(type_info.complete, getCompleteTypeIdentifier(), getCompleteTypeMap());
}

void TypeSupportImpl::populate_dependencies_i(const RcHandle<XTypes::TypeLookupService>& tls,
                                              XTypes::EquivalenceKind ek) const
{
  if (ek != XTypes::EK_MINIMAL && ek != XTypes::EK_COMPLETE) {
    return;
  }

  OPENDDS_SET(XTypes::TypeIdentifier) dependencies;
  const XTypes::TypeIdentifier& type_id = ek == XTypes::EK_MINIMAL ?
    getMinimalTypeIdentifier() : getCompleteTypeIdentifier();
  const XTypes::TypeMap& type_map = ek == XTypes::EK_MINIMAL ?
    getMinimalTypeMap() : getCompleteTypeMap();

  XTypes::compute_dependencies(type_map, type_id, dependencies);

  XTypes::TypeIdentifierWithSizeSeq deps_with_size;
  OPENDDS_SET(XTypes::TypeIdentifier)::const_iterator it = dependencies.begin();
  for (; it != dependencies.end(); ++it) {
    XTypes::TypeMap::const_iterator iter = type_map.find(*it);
    if (iter != type_map.end()) {
      const size_t tobj_size = serialized_size(XTypes::get_typeobject_encoding(), iter->second);
      XTypes::TypeIdentifierWithSize ti_with_size = {*it, static_cast<ACE_CDR::ULong>(tobj_size)};
      deps_with_size.append(ti_with_size);
    } else if (XTypes::has_type_object(*it)) {
      std::string kind = ek == XTypes::EK_MINIMAL ? "minimal" : "complete";
      ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) ERROR: TypeSupportImpl::populate_dependencies_i, ")
                 ACE_TEXT("local %C TypeIdentifier not found in local type map.\n"), kind.c_str()));
    }
  }
  tls->add_type_dependencies(type_id, deps_with_size);
}

void TypeSupportImpl::populate_dependencies(const RcHandle<XTypes::TypeLookupService>& tls) const
{
  populate_dependencies_i(tls, XTypes::EK_MINIMAL);
  populate_dependencies_i(tls, XTypes::EK_COMPLETE);
}

}
}

OPENDDS_END_VERSIONED_NAMESPACE_DECL
