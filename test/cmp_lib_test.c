/*
 * Copyright OpenSSL 2007-2018
 * Copyright Nokia 2007-2018
 * Copyright Siemens AG 2015-2018
 *
 * Contents licensed under the terms of the OpenSSL license
 * See https://www.openssl.org/source/license.html for details
 *
 * SPDX-License-Identifier: OpenSSL
 *
 * CMP tests by Martin Peylo, Tobias Pankert, and David von Oheimb.
 */

#include "cmptestlib.h"

typedef struct test_fixture {
    const char *test_case_name;
    int expected;
    CMP_CTX *cmp_ctx;
    CMP_PKIMESSAGE *msg;
    int callback_arg;
    int (*allow_unprotected_cb) (const CMP_CTX *, int, const CMP_PKIMESSAGE *);
    X509 *cert;
    STACK_OF(X509) *certs;
    STACK_OF(X509) *chain;
    ASN1_OCTET_STRING *src_string;
    ASN1_OCTET_STRING *tgt_string;
    long pkistatus;
    long pkifailure;
    char *text;                 /* Not freed by tear_down */

} CMP_LIB_TEST_FIXTURE;

static CMP_LIB_TEST_FIXTURE *set_up(const char *const test_case_name)
{
    CMP_LIB_TEST_FIXTURE *fixture;
    int setup_ok = 0;
    /* Allocate memory owned by the fixture, exit on error */
    if (!TEST_ptr(fixture = OPENSSL_zalloc(sizeof(*fixture))))
        goto err;
    fixture->test_case_name = test_case_name;
    if (!TEST_ptr(fixture->cmp_ctx = CMP_CTX_create()))
        goto err;
    setup_ok = 1;
 err:
    if (!setup_ok) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return fixture;
}

static void tear_down(CMP_LIB_TEST_FIXTURE *fixture)
{
    CMP_PKIMESSAGE_free(fixture->msg);
    CMP_CTX_delete(fixture->cmp_ctx);
    sk_X509_free(fixture->certs);
    sk_X509_free(fixture->chain);
    ASN1_OCTET_STRING_free(fixture->src_string);
    if (fixture->tgt_string != fixture->src_string)
        ASN1_OCTET_STRING_free(fixture->tgt_string);

    OPENSSL_free(fixture);
}

static EVP_PKEY *loadedkey = NULL;
static X509 *cert = NULL;
/* Certificates for trust chain */
static X509 *endentity1 = NULL, *endentity2 = NULL,
    *root = NULL, *intermediate = NULL;
static unsigned char rand_data[TRANSACTIONID_LENGTH];
static CMP_PKIMESSAGE *ir_unprotected, *ir_protected, *insta_unprotected;


static int allow_unprotected(const CMP_CTX *ctx, int arg,
                             const CMP_PKIMESSAGE *msg)
{
    return arg;
}

static int execute_protection_test(CMP_LIB_TEST_FIXTURE *fixture)
{
    return TEST_int_eq(fixture->expected,
                       CMP_PKIMESSAGE_protect(fixture->cmp_ctx, fixture->msg));
}

static int execute_check_received_test(CMP_LIB_TEST_FIXTURE *fixture)
{
    if (!TEST_int_eq(CMP_PKIMESSAGE_check_received(fixture->cmp_ctx,
                                                   fixture->msg,
                                                   fixture->callback_arg,
                                                   fixture->
                                                   allow_unprotected_cb),
                     fixture->expected))
        return 0;

    if (fixture->expected >= 0) {
        const CMP_PKIHEADER *header = CMP_PKIMESSAGE_get0_header(fixture->msg);
        if (!TEST_int_eq(0,
              ASN1_OCTET_STRING_cmp(CMP_PKIHEADER_get0_senderNonce(header),
                                    CMP_CTX_get0_recipNonce(fixture->cmp_ctx))))
            return 0;
        if (!TEST_int_eq(0,
           ASN1_OCTET_STRING_cmp(CMP_PKIHEADER_get0_transactionID(header),
                                 CMP_CTX_get0_transactionID(fixture->cmp_ctx))))
            return 0;
    }

    return 1;
}

static int execute_cmp_build_cert_chain_test(CMP_LIB_TEST_FIXTURE *fixture)
{
    STACK_OF(X509) *result = NULL;
    int ret = 0;
    if (TEST_ptr(result = CMP_build_cert_chain(fixture->certs, fixture->cert))) {
        /* Check whether chain built is equal to the expected one */
        ret = TEST_int_eq(0, STACK_OF_X509_cmp(result, fixture->chain));
        sk_X509_pop_free(result, X509_free);
    }
    return ret;
}

static int execute_cmp_asn1_octet_string_set_test(CMP_LIB_TEST_FIXTURE *
                                                  fixture)
{
    if (!TEST_int_eq(fixture->expected,
                     CMP_ASN1_OCTET_STRING_set1(&fixture->tgt_string,
                                                fixture->src_string)))
        return 0;
    if (fixture->expected)
        return TEST_int_eq(0, ASN1_OCTET_STRING_cmp(fixture->tgt_string,
                                                    fixture->src_string));
    return 1;
}

static int test_cmp_asn1_octet_string(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = 1;
    if (!TEST_ptr(fixture->tgt_string = ASN1_OCTET_STRING_new()) ||
        !TEST_ptr(fixture->src_string = ASN1_OCTET_STRING_new()) ||
        !TEST_true(ASN1_OCTET_STRING_set(fixture->src_string,
                rand_data, sizeof(rand_data)))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_asn1_octet_string_set_test, tear_down);
    return result;
}

static int test_cmp_asn1_octet_string_tgt_is_src(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = 1;
    if (!TEST_ptr(fixture->src_string = ASN1_OCTET_STRING_new()) ||
        !(fixture->tgt_string = fixture->src_string) ||
        !TEST_true(ASN1_OCTET_STRING_set(fixture->src_string, rand_data,
                sizeof(rand_data)))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_asn1_octet_string_set_test, tear_down);
    return result;
}

static int execute_cmp_pkimessage_add_extracerts_test(CMP_LIB_TEST_FIXTURE
                                                      * fixture)
{
    return TEST_true(CMP_PKIMESSAGE_add_extraCerts(fixture->cmp_ctx,
                                                   fixture->msg));
}

static int test_cmp_pkimessage_add_extracerts(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_protected))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_pkimessage_add_extracerts_test, tear_down);
    return result;
}

/*
 * Tests PKIStatusInfo creation and get-functions
 */
static int execute_cmp_pkistatusinfo_test(CMP_LIB_TEST_FIXTURE *fixture)
{
    CMP_PKISTATUSINFO *si = NULL;
    ASN1_UTF8STRING *statusString = NULL;
    CMP_PKIFAILUREINFO *fi = NULL;
    int res = 0, i;

    if (!TEST_ptr(si = CMP_statusInfo_new(fixture->pkistatus,
                                          fixture->pkifailure, fixture->text)))
        goto end;
    if (!TEST_long_eq(fixture->pkistatus,
                      CMP_PKISTATUSINFO_PKIStatus_get(si)) ||
        !TEST_long_eq(fixture->pkifailure,
                      CMP_PKISTATUSINFO_PKIFailureinfo_get(si)))
        goto end;
    if (!TEST_ptr(fi = CMP_PKISTATUSINFO_failInfo_get0(si)))
        goto end;
    for (i = 0; i <= CMP_PKIFAILUREINFO_MAX; i++)
        if (!TEST_int_eq(fixture->pkifailure >> i & 1,
                         CMP_PKIFAILUREINFO_check(fi, i)))
            goto end;
    if (!TEST_ptr
        (statusString =
         sk_ASN1_UTF8STRING_value(CMP_PKISTATUSINFO_statusString_get0(si), 0))
        || !TEST_str_eq(fixture->text, (char *)statusString->data))
        goto end;
    res = 1;
    /* TODO Wait for API consolidation */
 end:
    CMP_PKISTATUSINFO_free(si);
    return res;
}

static int
execute_cmp_pkimessage_set_and_check_implicit_confirm_test(CMP_LIB_TEST_FIXTURE
                                                           * fixture)
{
    return TEST_false(CMP_PKIMESSAGE_check_implicitConfirm(fixture->msg)) &&
        TEST_true(CMP_PKIMESSAGE_set_implicitConfirm(fixture->msg)) &&
        TEST_true(CMP_PKIMESSAGE_check_implicitConfirm(fixture->msg));
}

static int test_cmp_protection_unprotected_request(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    /* Do test case-specific set up; set expected return values and
     * side effects */
    fixture->expected = 1;
    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_true(CMP_CTX_set_option(fixture->cmp_ctx,
                                      CMP_CTX_OPT_UNPROTECTED_SEND, 1))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_protection_test, tear_down);
    return result;
}

static int test_cmp_protection_with_msg_sig_alg_protection_plus_rsa_key(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    const size_t size = sizeof(rand_data) / 2;
    fixture->expected = 1;

    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_true(CMP_CTX_set_option(fixture->cmp_ctx,
                                      CMP_CTX_OPT_UNPROTECTED_SEND, 0)) ||
        /* Use half of the 16 bytes of random input
         * for each reference and secret value */
        !TEST_true(CMP_CTX_set1_referenceValue(fixture->cmp_ctx, rand_data,
                size)) ||
        !TEST_true(CMP_CTX_set1_secretValue(fixture->cmp_ctx, rand_data + size,
                size))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_protection_test, tear_down);
    return result;
}

static int test_cmp_protection_with_certificate_and_key(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    /* Do test case-specific set up; set expected return values and
     * side effects */

    fixture->expected = 1;

    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_true(CMP_CTX_set_option(fixture->cmp_ctx,
                                      CMP_CTX_OPT_UNPROTECTED_SEND, 0)) ||
        !TEST_true(CMP_CTX_set1_pkey(fixture->cmp_ctx, loadedkey)) ||
        !TEST_true(CMP_CTX_set1_clCert(fixture->cmp_ctx, cert))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_protection_test, tear_down);
    return result;
}

static int test_cmp_protection_certificate_based_without_cert(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    /* Do test case-specific set up; set expected return values and
     * side effects */
    fixture->expected = 0;
    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_true(CMP_CTX_set_option(fixture->cmp_ctx,
                                      CMP_CTX_OPT_UNPROTECTED_SEND, 0)) ||
        !TEST_true(CMP_CTX_set1_newPkey(fixture->cmp_ctx, loadedkey))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_protection_test, tear_down);
    return result;
}

static int test_cmp_protection_no_key_no_secret(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    /* Do test case-specific set up; set expected return values and
     * side effects */
    fixture->expected = 0;
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_true(CMP_CTX_set_option(fixture->cmp_ctx,
                                      CMP_CTX_OPT_UNPROTECTED_SEND, 0))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_protection_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_set_and_check_implicit_confirm(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(ir_unprotected))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_pkimessage_set_and_check_implicit_confirm_test,
                 tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_no_protection_no_cb(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = -1;
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(ir_unprotected))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_no_protection_negative_cb(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = -1;
    fixture->callback_arg = 0;
    fixture->allow_unprotected_cb = allow_unprotected;
    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_no_protection_positive_cb(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = V_CMP_PKIBODY_IR;
    fixture->callback_arg = 1;
    fixture->allow_unprotected_cb = allow_unprotected;
    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_check_transaction_id(void)
{
    /* Transaction id belonging to CMP_IR_unprotected.der */
    const unsigned char trans_id[TRANSACTIONID_LENGTH] =
        { 0xDF, 0x5C, 0xDC, 0x01, 0xF8, 0x81, 0x6E, 0xA9,
        0x3E, 0x63, 0x94, 0x5B, 0xD3, 0x12, 0x1B, 0x65
    };
    ASN1_OCTET_STRING *trid = NULL;
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = V_CMP_PKIBODY_IR;
    fixture->callback_arg = 1;
    fixture->allow_unprotected_cb = allow_unprotected;
    if (!TEST_ptr(fixture->msg =
                  CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_ptr(trid = ASN1_OCTET_STRING_new()) ||
        !TEST_true(ASN1_OCTET_STRING_set(trid, trans_id, sizeof(trans_id))) ||
        !TEST_true(CMP_CTX_set1_transactionID(fixture->cmp_ctx, trid))) {
        tear_down(fixture);
        fixture = NULL;
    }
    ASN1_OCTET_STRING_free(trid);
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_wrong_transaction_id(void)
{
    ASN1_OCTET_STRING *trid = NULL;
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = -1;
    fixture->callback_arg = 1;
    fixture->allow_unprotected_cb = allow_unprotected;
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_ptr(trid = ASN1_OCTET_STRING_new()) ||
        !TEST_true(ASN1_OCTET_STRING_set(trid, rand_data, sizeof(rand_data))) ||
        !TEST_true(CMP_CTX_set1_transactionID(fixture->cmp_ctx, trid))) {
        tear_down(fixture);
        fixture = NULL;
    }
    ASN1_OCTET_STRING_free(trid);
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_wrong_recipient_nonce(void)
{
    ASN1_OCTET_STRING *snonce = NULL;
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = -1;
    fixture->callback_arg = 1;
    fixture->allow_unprotected_cb = allow_unprotected;
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(ir_unprotected)) ||
        !TEST_ptr(snonce = ASN1_OCTET_STRING_new()) ||
        !TEST_true(ASN1_OCTET_STRING_set(snonce, rand_data, sizeof(rand_data))) ||
        !TEST_true(CMP_CTX_set1_last_senderNonce(fixture->cmp_ctx, snonce))) {
        tear_down(fixture);
        fixture = NULL;
    }
    ASN1_OCTET_STRING_free(snonce);
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_check_received_check_recipient_nonce(void)
{
    /* Recipient nonce belonging to CMP_IP_insta_unprotected.der */
    const unsigned char rec_nonce[SENDERNONCE_LENGTH] =
        { 0x48, 0xF1, 0x71, 0x1F, 0xE5, 0xAF, 0x1C, 0x8B,
        0x21, 0x97, 0x5C, 0x84, 0x74, 0x49, 0xBA, 0x32
    };
    ASN1_OCTET_STRING *snonce = NULL;
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->expected = V_CMP_PKIBODY_IP;
    fixture->callback_arg = 1;
    fixture->allow_unprotected_cb = allow_unprotected;
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(insta_unprotected)) ||
        !TEST_ptr(snonce = ASN1_OCTET_STRING_new()) ||
        !TEST_true(ASN1_OCTET_STRING_set(snonce, rec_nonce, sizeof(rec_nonce)))
        || !TEST_true(CMP_CTX_set1_last_senderNonce(fixture->cmp_ctx, snonce))) {
        tear_down(fixture);
        fixture = NULL;
    }
    ASN1_OCTET_STRING_free(snonce);
    EXECUTE_TEST(execute_check_received_test, tear_down);
    return result;
}

static int test_cmp_pkistatusinfo(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->pkistatus = CMP_PKISTATUS_revocationNotification;
    fixture->pkifailure = CMP_CTX_FAILINFO_unsupportedVersion |
        CMP_CTX_FAILINFO_badDataFormat;
    fixture->text = "test_pki_free_text";
    EXECUTE_TEST(execute_cmp_pkistatusinfo_test, tear_down);
    return result;
}

static int test_cmp_pkimessage_get_and_check_implicit_confirm(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    if (!TEST_ptr(fixture->msg = CMP_PKIMESSAGE_dup(ir_unprotected))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_pkimessage_set_and_check_implicit_confirm_test,
                 tear_down);
    return result;
}

static int test_cmp_build_cert_chain(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->cert = endentity2;
    if (!TEST_ptr(fixture->certs = sk_X509_new_null()) ||
        !TEST_ptr(fixture->chain = sk_X509_new_null()) ||
        !TEST_true(sk_X509_push(fixture->certs, endentity1)) ||
        !TEST_true(sk_X509_push(fixture->certs, root)) ||
        !TEST_true(sk_X509_push(fixture->certs, intermediate)) ||
        !TEST_true(sk_X509_push(fixture->chain, endentity2)) ||
        !TEST_true(sk_X509_push(fixture->chain, intermediate))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_build_cert_chain_test, tear_down);
    return result;
}

static int test_cmp_build_cert_chain_missing_intermediate(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->cert = endentity2;
    if (!TEST_ptr(fixture->certs = sk_X509_new_null()) ||
        !TEST_ptr(fixture->chain = sk_X509_new_null()) ||
        !TEST_true(sk_X509_push(fixture->certs, endentity1)) ||
        !TEST_true(sk_X509_push(fixture->certs, root)) ||
        !TEST_true(sk_X509_push(fixture->chain, endentity2))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_build_cert_chain_test, tear_down);
    return result;
}

static int test_cmp_build_cert_chain_missing_root(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->cert = endentity2;
    if (!TEST_ptr(fixture->certs = sk_X509_new_null()) ||
        !TEST_ptr(fixture->chain = sk_X509_new_null()) ||
        !TEST_true(sk_X509_push(fixture->certs, endentity1)) ||
        !TEST_true(sk_X509_push(fixture->certs, intermediate)) ||
        !TEST_true(sk_X509_push(fixture->chain, endentity2)) ||
        !TEST_true(sk_X509_push(fixture->chain, intermediate))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_build_cert_chain_test, tear_down);
    return result;
}

static int test_cmp_build_cert_chain_no_certs(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->cert = endentity2;
    if (!TEST_ptr(fixture->certs = sk_X509_new_null()) ||
        !TEST_ptr(fixture->chain = sk_X509_new_null()) ||
        !TEST_true(sk_X509_push(fixture->chain, endentity2))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_build_cert_chain_test, tear_down);
    return result;
}

static int execute_cmp_x509_store_test(CMP_LIB_TEST_FIXTURE *fixture)
{
    X509_STORE *store = X509_STORE_new();
    STACK_OF(X509) *sk = NULL;
    int res = 0;
    if (!TEST_true
        (CMP_X509_STORE_add1_certs
         (store, fixture->certs, fixture->callback_arg)))
        goto err;
    if (!TEST_int_eq(0, STACK_OF_X509_cmp(sk = CMP_X509_STORE_get1_certs(store),
                                          fixture->chain)))
        goto err;
    res = 1;
 err:
    X509_STORE_free(store);
    sk_X509_pop_free(sk, X509_free);
    return res;

}

static int test_cmp_x509_store(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->certs = sk_X509_new_null();
    fixture->callback_arg = 0;  /* self-signed allowed */
    if (!TEST_true(sk_X509_push(fixture->certs, endentity1) &&
                   sk_X509_push(fixture->certs, endentity2) &&
                   sk_X509_push(fixture->certs, root) &&
                   sk_X509_push(fixture->certs, intermediate)) ||
        !TEST_ptr(fixture->chain = sk_X509_dup(fixture->certs))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_x509_store_test, tear_down);
    return result;
}

static int test_cmp_x509_store_only_self_signed(void)
{
    SETUP_TEST_FIXTURE(CMP_LIB_TEST_FIXTURE, set_up);
    fixture->certs = sk_X509_new_null();
    fixture->chain = sk_X509_new_null();
    fixture->callback_arg = 1;  /* only self-signed */
    if (!TEST_true(sk_X509_push(fixture->certs, endentity1) &&
                   sk_X509_push(fixture->certs, endentity2) &&
                   sk_X509_push(fixture->certs, root) &&
                   sk_X509_push(fixture->certs, intermediate)) ||
        !TEST_true(sk_X509_push(fixture->chain, root))) {
        tear_down(fixture);
        fixture = NULL;
    }
    EXECUTE_TEST(execute_cmp_x509_store_test, tear_down);
    return result;
}


void cleanup_tests(void)
{
    EVP_PKEY_free(loadedkey);
    X509_free(cert);
    X509_free(endentity1);
    X509_free(endentity2);
    X509_free(root);
    X509_free(intermediate);
    CMP_PKIMESSAGE_free(ir_protected);
    CMP_PKIMESSAGE_free(ir_unprotected);
    CMP_PKIMESSAGE_free(insta_unprotected);

    return;
}

int setup_tests(void)
{
    if(!TEST_int_eq(1, RAND_bytes(rand_data, TRANSACTIONID_LENGTH)))
        return 0;
    if (!TEST_ptr(endentity1 =
                  load_pem_cert("../cmp-test/chain/EndEntity1.crt")) ||
        !TEST_ptr(endentity2 =
                  load_pem_cert("../cmp-test/chain/EndEntity2.crt")) ||
        !TEST_ptr(root = load_pem_cert("../cmp-test/chain/Root_CA.crt")) ||
        !TEST_ptr(intermediate =
                  load_pem_cert("../cmp-test/chain/Intermediate_CA.crt")))
        return 0;
    if (!TEST_ptr(loadedkey = load_pem_key("../cmp-test/server.pem")) ||
        !TEST_ptr(cert =
                  load_pem_cert("../cmp-test/openssl_cmp_test_server.crt")))
        return 0;
    if (!TEST_ptr(ir_protected =
                load_pkimsg("../cmp-test/CMP_IR_protected.der")) ||
        !TEST_ptr(ir_unprotected =
                load_pkimsg("../cmp-test/CMP_IR_unprotected.der")) ||
        !TEST_ptr(insta_unprotected =
                load_pkimsg("../cmp-test/insta_removed_protection.der")))
        return 0;

    /* Message protection tests */
    ADD_TEST(test_cmp_protection_with_msg_sig_alg_protection_plus_rsa_key);
    ADD_TEST(test_cmp_protection_with_certificate_and_key);
    ADD_TEST(test_cmp_protection_certificate_based_without_cert);
    ADD_TEST(test_cmp_protection_unprotected_request);
    ADD_TEST(test_cmp_protection_no_key_no_secret);
    ADD_TEST(test_cmp_pkimessage_set_and_check_implicit_confirm);
    ADD_TEST(test_cmp_pkimessage_check_received_no_protection_no_cb);
    ADD_TEST(test_cmp_pkimessage_check_received_no_protection_negative_cb);
    ADD_TEST(test_cmp_pkimessage_check_received_no_protection_positive_cb);
    ADD_TEST(test_cmp_pkimessage_check_received_check_transaction_id);
    ADD_TEST(test_cmp_pkimessage_check_received_wrong_transaction_id);
    ADD_TEST(test_cmp_pkimessage_check_received_check_recipient_nonce);
    ADD_TEST(test_cmp_pkimessage_check_received_wrong_recipient_nonce);
    ADD_TEST(test_cmp_asn1_octet_string);
    ADD_TEST(test_cmp_asn1_octet_string_tgt_is_src);
    ADD_TEST(test_cmp_pkistatusinfo);
    ADD_TEST(test_cmp_pkimessage_get_and_check_implicit_confirm);
    ADD_TEST(test_cmp_pkimessage_add_extracerts);
    ADD_TEST(test_cmp_build_cert_chain);
    ADD_TEST(test_cmp_build_cert_chain_missing_root);
    ADD_TEST(test_cmp_build_cert_chain_missing_intermediate);
    ADD_TEST(test_cmp_build_cert_chain_no_certs);
    ADD_TEST(test_cmp_x509_store);
    ADD_TEST(test_cmp_x509_store_only_self_signed);
    /* TODO make sure that total number of tests (here currently 24) is shown,
     also for other cmp_*text.c. Currently the test drivers always show 1. */

    return 1;
}
