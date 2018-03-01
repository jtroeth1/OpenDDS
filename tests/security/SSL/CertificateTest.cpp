/*
 * Distributed under the DDS License.
 * See: http://www.DDS.org/license.html
 */

#include "gtest/gtest.h"
#include "dds/DCPS/security/SSL/Certificate.h"
#include <iostream>

using namespace OpenDDS::Security::SSL;

class CertificateTest : public ::testing::Test
{
public:
  CertificateTest() :
    ca_("file:../certs/opendds_identity_ca_cert.pem"),
    signed_("file:../certs/opendds_participant_cert.pem"),
    not_signed_("file:../certs/opendds_not_signed.pem")
  {

  }

  ~CertificateTest()
  {

  }

  Certificate ca_;
  Certificate signed_;
  Certificate not_signed_;
};

TEST_F(CertificateTest, Validate_Success)
{
  ASSERT_EQ(signed_.validate(ca_), X509_V_OK);
}

TEST_F(CertificateTest, Validate_Failure_LoadingWrongKeyType)
{
  Certificate wrong_key_type("file:../certs/opendds_participant_private_key.pem");
  ASSERT_NE(wrong_key_type.validate(ca_), X509_V_OK);
}

TEST_F(CertificateTest, Validate_Failure_SelfSigned)
{
  ASSERT_NE(not_signed_.validate(ca_), X509_V_OK);
}

#if 0 /* TODO */
TEST_F(CertificateTest, SubjectNameDigest)
{
  typedef std::vector<unsigned char> hash_vec;

  hash_vec hash;
  not_signed_.subject_name_digest(hash);

  hash_vec::const_iterator i;
  for (i = hash.cbegin(); i != hash.cend(); ++i) {
    /* Do something with the bytes... */
  }
}
#endif

TEST_F(CertificateTest, Algorithm_RSA_2048_Success)
{
  std::string algo;
  signed_.algorithm(algo);
  ASSERT_EQ(std::string("RSA-2048"), algo);
}



