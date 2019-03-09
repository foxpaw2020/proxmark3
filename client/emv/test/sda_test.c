/*
 * emv-tools - a set of tools to work with EMV family of smart cards
 * Copyright (C) 2012, 2015 Dmitry Eremin-Solenikov
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "../emv_pk.h"
#include "../crypto.h"
#include "../dump.h"
#include "../tlv.h"
#include "../emv_pki.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct emv_pk vsdc_01 = {
    .rid = { 0xa0, 0x00, 0x00, 0x00, 0x03, },
    .index = 1,
    .hash_algo = HASH_SHA_1,
    .pk_algo = PK_RSA,
    .hash = {
        0xd3, 0x4a, 0x6a, 0x77,
        0x60, 0x11, 0xc7, 0xe7,
        0xce, 0x3a, 0xec, 0x5f,
        0x03, 0xad, 0x2f, 0x8c,
        0xfc, 0x55, 0x03, 0xcc,
    },
    .exp = { 0x03, },
    .elen = 1,
    .mlen = 1024 / 8,
    .modulus = (unsigned char[])
    {
        0xc6, 0x96, 0x03, 0x42, 0x13, 0xd7, 0xd8, 0x54, 0x69, 0x84, 0x57, 0x9d, 0x1d, 0x0f, 0x0e, 0xa5,
        0x19, 0xcf, 0xf8, 0xde, 0xff, 0xc4, 0x29, 0x35, 0x4c, 0xf3, 0xa8, 0x71, 0xa6, 0xf7, 0x18, 0x3f,
        0x12, 0x28, 0xda, 0x5c, 0x74, 0x70, 0xc0, 0x55, 0x38, 0x71, 0x00, 0xcb, 0x93, 0x5a, 0x71, 0x2c,
        0x4e, 0x28, 0x64, 0xdf, 0x5d, 0x64, 0xba, 0x93, 0xfe, 0x7e, 0x63, 0xe7, 0x1f, 0x25, 0xb1, 0xe5,
        0xf5, 0x29, 0x85, 0x75, 0xeb, 0xe1, 0xc6, 0x3a, 0xa6, 0x17, 0x70, 0x69, 0x17, 0x91, 0x1d, 0xc2,
        0xa7, 0x5a, 0xc2, 0x8b, 0x25, 0x1c, 0x7e, 0xf4, 0x0f, 0x23, 0x65, 0x91, 0x24, 0x90, 0xb9, 0x39,
        0xbc, 0xa2, 0x12, 0x4a, 0x30, 0xa2, 0x8f, 0x54, 0x40, 0x2c, 0x34, 0xae, 0xca, 0x33, 0x1a, 0xb6,
        0x7e, 0x1e, 0x79, 0xb2, 0x85, 0xdd, 0x57, 0x71, 0xb5, 0xd9, 0xff, 0x79, 0xea, 0x63, 0x0b, 0x75,
    },
};

const unsigned char issuer_cert[] = {
    0x3c, 0x5f, 0xea, 0xd4, 0xdd, 0x7b, 0xca, 0x44, 0xf9, 0x3e, 0x90, 0xc4, 0x4f, 0x76, 0xed, 0xe5,
    0x4a, 0x32, 0x88, 0xec, 0xdc, 0x78, 0x46, 0x9f, 0xcb, 0x12, 0x25, 0xc0, 0x3b, 0x2c, 0x04, 0xf2,
    0xc2, 0xf4, 0x12, 0x28, 0x1a, 0x08, 0x22, 0xdf, 0x14, 0x64, 0x92, 0x30, 0x98, 0x9f, 0xb1, 0x49,
    0x40, 0x70, 0xda, 0xf8, 0xc9, 0x53, 0x4a, 0x78, 0x81, 0x96, 0x01, 0x48, 0x61, 0x6a, 0xce, 0x58,
    0x17, 0x88, 0x12, 0x0d, 0x35, 0x06, 0xac, 0xe4, 0xce, 0xe5, 0x64, 0xfb, 0x27, 0xee, 0x53, 0x34,
    0x1c, 0x22, 0xf0, 0xb4, 0x5b, 0x31, 0x87, 0x3d, 0x05, 0xde, 0x54, 0x5e, 0xfe, 0x33, 0xbc, 0xd2,
    0x9b, 0x21, 0x85, 0xd0, 0x35, 0xa8, 0x06, 0xad, 0x08, 0xc6, 0x97, 0x6f, 0x35, 0x05, 0xa1, 0x99,
    0x99, 0x93, 0x0c, 0xa8, 0xa0, 0x3e, 0xfa, 0x32, 0x1c, 0x48, 0x60, 0x61, 0xf7, 0xdc, 0xec, 0x9f,
};

const unsigned char issuer_rem[] = {
    0x1e, 0xbc, 0xa3, 0x0f, 0x00, 0xce, 0x59, 0x62, 0xa8, 0xc6, 0xe1, 0x30, 0x54, 0x4b, 0x82, 0x89,
    0x1b, 0x23, 0x6c, 0x65, 0xde, 0x29, 0x31, 0x7f, 0x36, 0x47, 0x35, 0xde, 0xe6, 0x3f, 0x65, 0x98,
    0x97, 0x58, 0x35, 0xd5
};

const unsigned char issuer_exp[] = {
    0x03,
};

const unsigned char ssad_cr[] = {
    0x99, 0xa5, 0x58, 0xb6, 0x2b, 0x67, 0x4a, 0xa5, 0xe7, 0xd2, 0xa5, 0x7e, 0x5e, 0xf6, 0xa6, 0xf2,
    0x25, 0x8e, 0x5d, 0xa0, 0x52, 0xd0, 0x5b, 0x54, 0xe5, 0xc1, 0x15, 0xff, 0x1c, 0xec, 0xf9, 0x4a,
    0xa2, 0xdf, 0x8f, 0x39, 0xa0, 0x1d, 0x71, 0xc6, 0x19, 0xeb, 0x81, 0x9d, 0xa5, 0x2e, 0xf3, 0x81,
    0xe8, 0x49, 0x79, 0x58, 0x6a, 0xea, 0x78, 0x55, 0xff, 0xbe, 0xf4, 0x0a, 0xa3, 0xa7, 0x1c, 0xd3,
    0xb0, 0x4c, 0xfd, 0xf2, 0x70, 0xae, 0xc8, 0x15, 0x8a, 0x27, 0x97, 0xf2, 0x4f, 0xd6, 0x13, 0xb7,
    0x48, 0x13, 0x46, 0x61, 0x13, 0x5c, 0xd2, 0x90, 0xe4, 0x5b, 0x04, 0xa8, 0xe0, 0xcc, 0xc7, 0x11,
    0xae, 0x04, 0x2f, 0x15, 0x9e, 0x73, 0xc8, 0x9c, 0x2a, 0x7e, 0x65, 0xa4, 0xc2, 0xfd, 0x1d, 0x61,
    0x06, 0x02, 0x4a, 0xa2, 0x71, 0x30, 0xb0, 0xec, 0xec, 0x02, 0x38, 0xf9, 0x16, 0x59, 0xde, 0x96,
};

const unsigned char ssd1[] = {
    0x5f, 0x24, 0x03, 0x08, 0x12, 0x31, 0x5a, 0x08, 0x42, 0x76, 0x55, 0x00, 0x13, 0x23, 0x45, 0x99, 0x5f, 0x34, 0x01, 0x01, 0x9f, 0x07, 0x02, 0xff, 0x00, 0x9f, 0x0d, 0x05, 0xd0, 0x40, 0xac, 0xa8, 0x00, 0x9f, 0x0e, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x9f, 0x0f, 0x05, 0xd0, 0x68, 0xbc, 0xf8, 0x00,
    0x5c, 0x00,
};
static const struct tlv ssd1_tlv = {
    .len = sizeof(ssd1),
    .value = ssd1,
};

const unsigned char pan[] = {
    0x42, 0x76, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static int sda_test_raw(bool verbose)
{
    const struct emv_pk *pk = &vsdc_01;

    struct crypto_pk *kcp = crypto_pk_open(PK_RSA,
                                           pk->modulus, pk->mlen,
                                           pk->exp, pk->elen);
    if (!kcp)
        return 1;

    unsigned char *ipk_data;
    size_t ipk_data_len;
    ipk_data = crypto_pk_encrypt(kcp, issuer_cert, sizeof(issuer_cert), &ipk_data_len);
    crypto_pk_close(kcp);

    if (!ipk_data)
        return 1;

    if (verbose) {
        printf("issuer cert:\n");
        dump_buffer(ipk_data, ipk_data_len, stdout, 0);
    }

    size_t ipk_pk_len = ipk_data[13];
    unsigned char *ipk_pk = malloc(ipk_pk_len);
    memcpy(ipk_pk, ipk_data + 15, ipk_data_len - 36);
    memcpy(ipk_pk + ipk_data_len - 36, issuer_rem, sizeof(issuer_rem));

    struct crypto_hash *ch;
    ch = crypto_hash_open(HASH_SHA_1);
    if (!ch) {
        free(ipk_pk);
        free(ipk_data);
        return 1;
    }

    crypto_hash_write(ch, ipk_data + 1, 14);
    crypto_hash_write(ch, ipk_pk, ipk_pk_len);
    crypto_hash_write(ch, issuer_exp, sizeof(issuer_exp));

    unsigned char *h = crypto_hash_read(ch);
    if (!h) {
        crypto_hash_close(ch);
        free(ipk_pk);
        free(ipk_data);
        return 1;
    }

    if (verbose) {
        printf("crypto hash:\n");
        dump_buffer(h, 20, stdout, 0);
    }

    if (memcmp(ipk_data + ipk_data_len - 21, h, 20)) {
        crypto_hash_close(ch);
        free(ipk_pk);
        free(ipk_data);
        return 1;
    }

    crypto_hash_close(ch);
    free(ipk_data);

    struct crypto_pk *ikcp = crypto_pk_open(PK_RSA, ipk_pk, (int) ipk_pk_len,
                                            issuer_exp, (int) sizeof(issuer_exp));
    free(ipk_pk);
    if (!ikcp)
        return 1;

    size_t ssad_len;
    unsigned char *ssad = crypto_pk_encrypt(ikcp, ssad_cr, sizeof(ssad_cr), &ssad_len);
    crypto_pk_close(ikcp);
    if (!ssad)
        return 1;

    if (verbose) {
        printf("ssad:\n");
        dump_buffer(ssad, ssad_len, stdout, 0);
    }

    ch = crypto_hash_open(HASH_SHA_1);
    if (!ch) {
        free(ssad);
        return 1;
    }

    crypto_hash_write(ch, ssad + 1, ssad_len - 22);
    crypto_hash_write(ch, ssd1, sizeof(ssd1));

    unsigned char *h2 = crypto_hash_read(ch);
    if (!h2) {
        crypto_hash_close(ch);
        free(ssad);
        return 1;
    }

    if (verbose) {
        printf("crypto hash2:\n");
        dump_buffer(h2, 20, stdout, 0);
    }

    crypto_hash_close(ch);

    free(ssad);

    return 0;
}

static int sda_test_pk(bool verbose)
{
    const struct emv_pk *pk = &vsdc_01;
    struct tlvdb *db;

    db = tlvdb_external(0x90, sizeof(issuer_cert), issuer_cert);
    tlvdb_add(db, tlvdb_external(0x9f32, sizeof(issuer_exp), issuer_exp));
    tlvdb_add(db, tlvdb_external(0x92, sizeof(issuer_rem), issuer_rem));
    tlvdb_add(db, tlvdb_external(0x5a, sizeof(pan), pan));

    struct emv_pk *ipk = emv_pki_recover_issuer_cert(pk, db);
    if (!ipk) {
        fprintf(stderr, "Could not recover Issuer certificate!\n");
        tlvdb_free(db);
        return 2;
    }

    tlvdb_add(db, tlvdb_external(0x93, sizeof(ssad_cr), ssad_cr));

    struct tlvdb *dacdb = emv_pki_recover_dac(ipk, db, &ssd1_tlv);
    if (!dacdb) {
        fprintf(stderr, "Could not recover DAC!\n");
        emv_pk_free(ipk);
        tlvdb_free(db);
        return 2;
    }

    const struct tlv *dac = tlvdb_get(dacdb, 0x9f45, NULL);
    if (!dac) {
        fprintf(stderr, "DAC not found!\n");
        tlvdb_free(dacdb);
        emv_pk_free(ipk);
        tlvdb_free(db);
        return 2;
    }

    if (verbose) {
        printf("dac:\n");
        dump_buffer(dac->value, dac->len, stdout, 0);
    }

    tlvdb_free(dacdb);
    emv_pk_free(ipk);
    tlvdb_free(db);

    return 0;
}

int exec_sda_test(bool verbose)
{
    int ret;
    fprintf(stdout, "\n");

    ret = sda_test_raw(verbose);
    if (ret) {
        fprintf(stderr, "SDA raw test: failed\n");
        return ret;
    }
    fprintf(stdout, "SDA raw test: passed\n");

    ret = sda_test_pk(verbose);
    if (ret) {
        fprintf(stderr, "SDA test pk: failed\n");
        return ret;
    }
    fprintf(stdout, "SDA test pk: passed\n");

    return 0;
}
