// -*- C++ -*-
#include "SerializerTest2TypeSupportImpl.h"
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/Marked_Default_Qos.h>
#include "dds/DCPS/StaticIncludes.h"

#include "dds/DCPS/Serializer.h"
#include <Message_Block.h>

#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/RTPS/RtpsDiscovery.h>
#include <dds/DCPS/transport/rtps_udp/RtpsUdp.h>
#endif

#include <tests/Utils/DistributedConditionSet.h>

const auto encoding = OpenDDS::DCPS::Encoding(OpenDDS::DCPS::Encoding::KIND_XCDR2, OpenDDS::DCPS::ENDIAN_NATIVE);
//char* serializedRaw = 0;
//size_t dataSize = 0;
//ACE_Message_Block          originalMb(dataSize);

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

    std::cout << "start serialize()\n";

    SerializerTest2::Message message;
    message.bytes.length(10);

    //size_t dataSize = 0;
    OpenDDS::DCPS::serialized_size(encoding, dataSize, data);
    ACE_Message_Block mb(dataSize);

    auto serializer = OpenDDS::DCPS::Serializer(&mb, encoding.kind(), encoding.endianness());
    std::cout << "mb=" << serializer.current() << std::endl;

    std::cout << "current mb=" << serializer.current() << std::endl;
    std::cout << "mb size=" << mb.size() << "\n";

    if (!(serializer << data))
        std::cerr << "\tError serializing " << "mb size=" << mb.size() << " dataSize=" << dataSize << std::endl;



    return 0;
}
