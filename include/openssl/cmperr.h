/*
 * Generated by util/mkerr.pl DO NOT EDIT
 * Copyright 1995-2018 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

#ifndef HEADER_CMPERR_H
# define HEADER_CMPERR_H

# include <openssl/opensslconf.h>

# ifndef OPENSSL_NO_CMP

#  ifdef  __cplusplus
extern "C"
#  endif
int ERR_load_CMP_strings(void);

/*
 * CMP function codes.
 */
#  define CMP_F_CMP_ASN1_OCTET_STRING_SET1                 100
#  define CMP_F_CMP_ASN1_OCTET_STRING_SET1_BYTES           199
#  define CMP_F_CMP_CALC_PROTECTION                        101
#  define CMP_F_CMP_CERTCONF_NEW                           102
#  define CMP_F_CMP_CERTORENCCERT_ENCCERT_GET1             103
#  define CMP_F_CMP_CERTREPMESSAGE_CERTRESPONSE_GET0       104
#  define CMP_F_CMP_CERTREP_NEW                            184
#  define CMP_F_CMP_CERTREQ_NEW                            105
#  define CMP_F_CMP_CERTRESPONSE_GET_CERTIFICATE           106
#  define CMP_F_CMP_CERTRESPONSE_NEW                       182
#  define CMP_F_CMP_CERTSTATUS_SET_CERTHASH                107
#  define CMP_F_CMP_CTX_CAPUBS_GET1                        108
#  define CMP_F_CMP_CTX_CAPUBS_NUM                         109
#  define CMP_F_CMP_CTX_CAPUBS_POP                         110
#  define CMP_F_CMP_CTX_CAPUBS_SET0                        179
#  define CMP_F_CMP_CTX_CREATE                             111
#  define CMP_F_CMP_CTX_EXTRACERTSIN_GET1                  112
#  define CMP_F_CMP_CTX_EXTRACERTSIN_NUM                   113
#  define CMP_F_CMP_CTX_EXTRACERTSIN_POP                   114
#  define CMP_F_CMP_CTX_EXTRACERTSOUT_NUM                  115
#  define CMP_F_CMP_CTX_EXTRACERTSOUT_PUSH1                116
#  define CMP_F_CMP_CTX_INIT                               117
#  define CMP_F_CMP_CTX_PUSH_FREETEXT                      206
#  define CMP_F_CMP_CTX_SET0_NEWPKEY                       118
#  define CMP_F_CMP_CTX_SET0_PKEY                          119
#  define CMP_F_CMP_CTX_SET0_REQEXTENSIONS                 120
#  define CMP_F_CMP_CTX_SET0_TLSBIO                        121
#  define CMP_F_CMP_CTX_SET1_CAPUBS                        122
#  define CMP_F_CMP_CTX_SET1_CLCERT                        123
#  define CMP_F_CMP_CTX_SET1_EXPECTED_SENDER               124
#  define CMP_F_CMP_CTX_SET1_EXTRACERTSIN                  125
#  define CMP_F_CMP_CTX_SET1_EXTRACERTSOUT                 126
#  define CMP_F_CMP_CTX_SET1_ISSUER                        127
#  define CMP_F_CMP_CTX_SET1_LAST_SENDERNONCE              180
#  define CMP_F_CMP_CTX_SET1_NEWCLCERT                     128
#  define CMP_F_CMP_CTX_SET1_NEWPKEY                       129
#  define CMP_F_CMP_CTX_SET1_OLDCLCERT                     130
#  define CMP_F_CMP_CTX_SET1_P10CSR                        131
#  define CMP_F_CMP_CTX_SET1_PKEY                          132
#  define CMP_F_CMP_CTX_SET1_PROXYNAME                     133
#  define CMP_F_CMP_CTX_SET1_RECIPIENT                     134
#  define CMP_F_CMP_CTX_SET1_RECIPNONCE                    135
#  define CMP_F_CMP_CTX_SET1_REFERENCEVALUE                136
#  define CMP_F_CMP_CTX_SET1_SECRETVALUE                   137
#  define CMP_F_CMP_CTX_SET1_SERVERNAME                    138
#  define CMP_F_CMP_CTX_SET1_SERVERPATH                    139
#  define CMP_F_CMP_CTX_SET1_SRVCERT                       140
#  define CMP_F_CMP_CTX_SET1_SUBJECTNAME                   141
#  define CMP_F_CMP_CTX_SET1_TRANSACTIONID                 142
#  define CMP_F_CMP_CTX_SET_PROXYPORT                      143
#  define CMP_F_CMP_CTX_SET_SERVERPORT                     144
#  define CMP_F_CMP_CTX_SUBJECTALTNAME_PUSH1               145
#  define CMP_F_CMP_ERROR_NEW                              146
#  define CMP_F_CMP_EXCHANGE_CERTCONF                      171
#  define CMP_F_CMP_EXCHANGE_ERROR                         175
#  define CMP_F_CMP_EXEC_CR_SES                            147
#  define CMP_F_CMP_EXEC_GENM_SES                          148
#  define CMP_F_CMP_EXEC_IR_SES                            149
#  define CMP_F_CMP_EXEC_KUR_SES                           150
#  define CMP_F_CMP_EXEC_P10CR_SES                         151
#  define CMP_F_CMP_EXEC_RR_SES                            152
#  define CMP_F_CMP_GENM_NEW                               153
#  define CMP_F_CMP_GENP_NEW                               186
#  define CMP_F_CMP_PKICONF_NEW                            187
#  define CMP_F_CMP_PKIFREETEXT_PUSH_STR                   198
#  define CMP_F_CMP_PKIHEADER_GENERALINFO_ITEM_PUSH0       154
#  define CMP_F_CMP_PKIHEADER_INIT                         200
#  define CMP_F_CMP_PKIHEADER_PUSH0_FREETEXT               201
#  define CMP_F_CMP_PKIHEADER_PUSH1_FREETEXT               202
#  define CMP_F_CMP_PKIHEADER_SET_MESSAGETIME              203
#  define CMP_F_CMP_PKIHEADER_SET_VERSION                  204
#  define CMP_F_CMP_PKIMESSAGE_CHECK_RECEIVED              155
#  define CMP_F_CMP_PKIMESSAGE_CREATE                      183
#  define CMP_F_CMP_PKIMESSAGE_GENERALINFO_ITEMS_PUSH1     156
#  define CMP_F_CMP_PKIMESSAGE_GENM_ITEMS_PUSH1            157
#  define CMP_F_CMP_PKIMESSAGE_GENM_ITEM_PUSH0             158
#  define CMP_F_CMP_PKIMESSAGE_HTTP_PERFORM                159
#  define CMP_F_CMP_PKIMESSAGE_POLLRESPONSE_GET0           160
#  define CMP_F_CMP_PKIMESSAGE_PROTECT                     161
#  define CMP_F_CMP_PKISTATUSINFO_PKIFAILUREINFO_GET       197
#  define CMP_F_CMP_PKISTATUSINFO_PKISTATUS_GET            162
#  define CMP_F_CMP_PKISTATUSINFO_PKISTATUS_GET_STRING     163
#  define CMP_F_CMP_POLLREPCONTENT_POLLREP_GET0            166
#  define CMP_F_CMP_POLLREP_NEW                            188
#  define CMP_F_CMP_POLLREQ_NEW                            164
#  define CMP_F_CMP_PROCESS_CERT_REQUEST                   185
#  define CMP_F_CMP_REVREPCONTENT_PKISTATUSINFO_GET        165
#  define CMP_F_CMP_RP_NEW                                 189
#  define CMP_F_CMP_RR_NEW                                 169
#  define CMP_F_CMP_SRV_CTX_CREATE                         190
#  define CMP_F_CMP_VALIDATE_CERT_PATH                     167
#  define CMP_F_CMP_VALIDATE_MSG                           168
#  define CMP_F_CMP_VERIFY_PBMAC                           172
#  define CMP_F_CMP_VERIFY_POPO                            196
#  define CMP_F_CMP_VERIFY_SIGNATURE                       170
#  define CMP_F_FIND_SRVCERT                               173
#  define CMP_F_GET_CERT_STATUS                            174
#  define CMP_F_POLLFORRESPONSE                            178
#  define CMP_F_PROCESS_CERTCONF                           191
#  define CMP_F_PROCESS_ERROR                              192
#  define CMP_F_PROCESS_GENM                               193
#  define CMP_F_PROCESS_POLLREQ                            194
#  define CMP_F_PROCESS_REQUEST                            176
#  define CMP_F_PROCESS_RR                                 195
#  define CMP_F_SEND_RECEIVE_CHECK                         177
#  define CMP_F_SET1_AOSTR_ELSE_RANDOM                     181
#  define CMP_F_SET1_GENERAL_NAME                          205

/*
 * CMP reason codes.
 */
#  define CMP_R_ALGORITHM_NOT_SUPPORTED                    100
#  define CMP_R_CERTIFICATE_NOT_ACCEPTED                   101
#  define CMP_R_CERTIFICATE_NOT_FOUND                      102
#  define CMP_R_CERTREQMSG_NOT_FOUND                       182
#  define CMP_R_CERTRESPONSE_NOT_FOUND                     103
#  define CMP_R_CERT_AND_KEY_DO_NOT_MATCH                  104
#  define CMP_R_CONNECT_TIMEOUT                            105
#  define CMP_R_CP_NOT_RECEIVED                            106
#  define CMP_R_ENCOUNTERED_KEYUPDATEWARNING               107
#  define CMP_R_ENCOUNTERED_UNSUPPORTED_PKISTATUS          108
#  define CMP_R_ENCOUNTERED_WAITING                        109
#  define CMP_R_ERROR_CALCULATING_PROTECTION               110
#  define CMP_R_ERROR_CONNECTING                           111
#  define CMP_R_ERROR_CREATING_CERTCONF                    112
#  define CMP_R_ERROR_CREATING_CERTREP                     183
#  define CMP_R_ERROR_CREATING_CR                          113
#  define CMP_R_ERROR_CREATING_ERROR                       114
#  define CMP_R_ERROR_CREATING_GENM                        115
#  define CMP_R_ERROR_CREATING_GENP                        184
#  define CMP_R_ERROR_CREATING_IR                          116
#  define CMP_R_ERROR_CREATING_KUR                         117
#  define CMP_R_ERROR_CREATING_P10CR                       118
#  define CMP_R_ERROR_CREATING_PKICONF                     185
#  define CMP_R_ERROR_CREATING_POLLREP                     186
#  define CMP_R_ERROR_CREATING_POLLREQ                     119
#  define CMP_R_ERROR_CREATING_RP                          187
#  define CMP_R_ERROR_CREATING_RR                          120
#  define CMP_R_ERROR_DECODING_CERTIFICATE                 121
#  define CMP_R_ERROR_DECODING_MESSAGE                     122
#  define CMP_R_ERROR_DECRYPTING_CERTIFICATE               123
#  define CMP_R_ERROR_DECRYPTING_ENCCERT                   124
#  define CMP_R_ERROR_DECRYPTING_KEY                       125
#  define CMP_R_ERROR_DECRYPTING_SYMMETRIC_KEY             126
#  define CMP_R_ERROR_LEARNING_TRANSACTIONID               179
#  define CMP_R_ERROR_PARSING_PKISTATUS                    127
#  define CMP_R_ERROR_PROCESSING_CERTREQ                   188
#  define CMP_R_ERROR_PROCESSING_MSG                       189
#  define CMP_R_ERROR_PROTECTING_MESSAGE                   128
#  define CMP_R_ERROR_PUSHING_GENERALINFO_ITEM             129
#  define CMP_R_ERROR_PUSHING_GENERALINFO_ITEMS            130
#  define CMP_R_ERROR_PUSHING_GENM_ITEMS                   131
#  define CMP_R_ERROR_SENDING_REQUEST                      132
#  define CMP_R_ERROR_SETTING_CERTHASH                     133
#  define CMP_R_ERROR_TRANSFERRING_IN                      134
#  define CMP_R_ERROR_TRANSFERRING_OUT                     135
#  define CMP_R_ERROR_VALIDATING_PROTECTION                136
#  define CMP_R_FAILED_EXTRACTING_PUBKEY                   198
#  define CMP_R_FAILED_TO_RECEIVE_PKIMESSAGE               137
#  define CMP_R_FAILED_TO_SEND_REQUEST                     138
#  define CMP_R_FAILED_TO_VERIFY_REQUEST                   190
#  define CMP_R_FAILURE_OBTAINING_RANDOM                   178
#  define CMP_R_GENP_NOT_RECEIVED                          139
#  define CMP_R_INVALID_ARGS                               140
#  define CMP_R_INVALID_CONTEXT                            141
#  define CMP_R_INVALID_KEY                                144
#  define CMP_R_INVALID_PARAMETERS                         143
#  define CMP_R_IP_NOT_RECEIVED                            145
#  define CMP_R_KUP_NOT_RECEIVED                           146
#  define CMP_R_MISSING_KEY_INPUT_FOR_CREATING_PROTECTION  147
#  define CMP_R_MISSING_KEY_USAGE_DIGITALSIGNATUE          176
#  define CMP_R_MISSING_PROTECTION                         181
#  define CMP_R_MULTIPLE_SAN_SOURCES                       180
#  define CMP_R_NO_NULL_ARGUMENT                           195
#  define CMP_R_NO_SENDER_NO_REFERENCE                     196
#  define CMP_R_NO_SUITABLE_SERVER_CERT                    199
#  define CMP_R_NO_VALID_SERVER_CERT_FOUND                 148
#  define CMP_R_NULL_ARGUMENT                              149
#  define CMP_R_OUT_OF_MEMORY                              150
#  define CMP_R_PKIBODY_ERROR                              151
#  define CMP_R_PKICONF_NOT_RECEIVED                       152
#  define CMP_R_PKISTATUSINFO_NOT_FOUND                    153
#  define CMP_R_POLLREP_NOT_RECEIVED                       154
#  define CMP_R_POTENTIALLY_INVALID_CERTIFICATE            142
#  define CMP_R_READ_TIMEOUT                               155
#  define CMP_R_RECEIVED_ERROR                             156
#  define CMP_R_RECEIVED_NEGATIVE_CHECKAFTER_IN_POLLREP    157
#  define CMP_R_RECIPNONCE_UNMATCHED                       158
#  define CMP_R_REQUEST_NOT_ACCEPTED                       194
#  define CMP_R_REQUEST_REJECTED_BY_CA                     159
#  define CMP_R_RP_NOT_RECEIVED                            160
#  define CMP_R_SENDER_GENERALNAME_TYPE_NOT_SUPPORTED      161
#  define CMP_R_SERVER_NOT_REACHABLE                       162
#  define CMP_R_TLS_ERROR                                  163
#  define CMP_R_TOTAL_TIMEOUT                              197
#  define CMP_R_TRANSACTIONID_UNMATCHED                    164
#  define CMP_R_UNEXPECTED_PKIBODY                         165
#  define CMP_R_UNEXPECTED_PKISTATUS                       166
#  define CMP_R_UNEXPECTED_REQUEST_ID                      191
#  define CMP_R_UNEXPECTED_SENDER                          167
#  define CMP_R_UNKNOWN_ALGORITHM_ID                       168
#  define CMP_R_UNKNOWN_CERT_TYPE                          169
#  define CMP_R_UNKNOWN_PKISTATUS                          170
#  define CMP_R_UNSUPPORTED_ALGORITHM                      171
#  define CMP_R_UNSUPPORTED_CIPHER                         172
#  define CMP_R_UNSUPPORTED_KEY_TYPE                       173
#  define CMP_R_UNSUPPORTED_POPO_METHOD                    192
#  define CMP_R_UNSUPPORTED_PROTECTION_ALG_DHBASEDMAC      174
#  define CMP_R_WRONG_ALGORITHM_OID                        175
#  define CMP_R_WRONG_CERT_HASH                            193
#  define CMP_R_WRONG_PBM_VALUE                            177

# endif
#endif
