// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_HASH_H
#define BITCOIN_HASH_H

#include "uint256.h"
#include "serialize.h"

#include <openssl/evp.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <vector>

#ifndef OPENSSL_NO_DEPRECATED
#define OPENSSL_NO_DEPRECATED
#endif

template<typename T1>
inline uint256 Hash(const T1 pbegin, const T1 pend)
{
    static unsigned char pblank[1];
    uint256 hash1;
    SHA256((pbegin == pend ? pblank : (unsigned char*)&pbegin[0]), (pend - pbegin) * sizeof(pbegin[0]), (unsigned char*)&hash1);
    uint256 hash2;
    SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&hash2);
    return hash2;
}

class CHashWriter
{
private:
#ifndef OPENSSL_NO_DEPRECATED
    SHA256_CTX ctx;
#else
    EVP_MD_CTX* _ctx;
#endif
public:
    int nType;
    int nVersion;

    void Init() {
#ifndef OPENSSL_NO_DEPRECATED
        SHA256_Init(&ctx);
#else
        _ctx = EVP_MD_CTX_create();
        EVP_DigestInit_ex(_ctx, EVP_sha256(), nullptr);
#endif
    }

    CHashWriter(int nTypeIn, int nVersionIn)
        : nType(nTypeIn)
        , nVersion(nVersionIn)
    {
        Init();
    }

    ~CHashWriter() {
        EVP_MD_CTX_destroy(_ctx);
    }
    CHashWriter& write(const char *pch, size_t size) {
# ifndef OPENSSL_NO_DEPRECATED
        SHA256_Update(&ctx, pch, size);
#else
        EVP_DigestUpdate(_ctx, pch, size);
#endif
        return (*this);
    }

    // invalidates the object
    uint256 GetHash() {
# ifndef OPENSSL_NO_DEPRECATED
        uint256 hash1;
        SHA256_Final((unsigned char*)&hash1, &ctx);
#else
        uint256 hash1;
        EVP_DigestFinal_ex(_ctx, (unsigned char*)&hash1, nullptr);
#endif
        uint256 hash2;
        SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&hash2);
        return hash2;
    }

    template<typename T>
    CHashWriter& operator<<(const T& obj) {
        // Serialize to this stream
        ::Serialize(*this, obj, nType, nVersion);
        return (*this);
    }
};


template<typename T1, typename T2>
inline uint256 Hash(const T1 p1begin, const T1 p1end,
                    const T2 p2begin, const T2 p2end)
{
    static unsigned char pblank[1];
    uint256 hash1;
# ifndef OPENSSL_NO_DEPRECATED
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (p1begin == p1end ? pblank : (unsigned char*)&p1begin[0]), (p1end - p1begin) * sizeof(p1begin[0]));
    SHA256_Update(&ctx, (p2begin == p2end ? pblank : (unsigned char*)&p2begin[0]), (p2end - p2begin) * sizeof(p2begin[0]));
    SHA256_Final((unsigned char*)&hash1, &ctx);
#else
    auto md_ctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(md_ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(md_ctx, (p1begin == p1end ? pblank : (unsigned char*)&p1begin[0]), (p1end - p1begin) * sizeof(p1begin[0]));
    EVP_DigestUpdate(md_ctx, (p2begin == p2end ? pblank : (unsigned char*)&p2begin[0]), (p2end - p2begin) * sizeof(p2begin[0]));
    EVP_DigestFinal_ex(md_ctx, (unsigned char*)&hash1, nullptr);
    EVP_MD_CTX_destroy(md_ctx);
#endif
    uint256 hash2;
    SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&hash2);
    return hash2;
}

template<typename T1, typename T2, typename T3>
inline uint256 Hash(const T1 p1begin, const T1 p1end,
                    const T2 p2begin, const T2 p2end,
                    const T3 p3begin, const T3 p3end)
{
    static unsigned char pblank[1];
    uint256 hash1;
# ifndef OPENSSL_NO_DEPRECATED
    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, (p1begin == p1end ? pblank : (unsigned char*)&p1begin[0]), (p1end - p1begin) * sizeof(p1begin[0]));
    SHA256_Update(&ctx, (p2begin == p2end ? pblank : (unsigned char*)&p2begin[0]), (p2end - p2begin) * sizeof(p2begin[0]));
    SHA256_Update(&ctx, (p3begin == p3end ? pblank : (unsigned char*)&p3begin[0]), (p3end - p3begin) * sizeof(p3begin[0]));
    SHA256_Final((unsigned char*)&hash1, &ctx);
#else
    auto md_ctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(md_ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(md_ctx, (p1begin == p1end ? pblank : (unsigned char*)&p1begin[0]), (p1end - p1begin) * sizeof(p1begin[0]));
    EVP_DigestUpdate(md_ctx, (p2begin == p2end ? pblank : (unsigned char*)&p2begin[0]), (p2end - p2begin) * sizeof(p2begin[0]));
    EVP_DigestUpdate(md_ctx, (p3begin == p3end ? pblank : (unsigned char*)&p3begin[0]), (p3end - p3begin) * sizeof(p3begin[0]));
    EVP_DigestFinal_ex(md_ctx, (unsigned char*)&hash1, nullptr);
    EVP_MD_CTX_destroy(md_ctx);
#endif
    uint256 hash2;
    SHA256((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&hash2);
    return hash2;
}

template<typename T>
uint256 SerializeHash(const T& obj, int nType=SER_GETHASH, int nVersion=PROTOCOL_VERSION)
{
    CHashWriter ss(nType, nVersion);
    ss << obj;
    return ss.GetHash();
}

template<typename T1>
inline uint160 Hash160(const T1 pbegin, const T1 pend)
{
    static unsigned char pblank[1];
    uint256 hash1;
    SHA256((pbegin == pend ? pblank : (unsigned char*)&pbegin[0]), (pend - pbegin) * sizeof(pbegin[0]), (unsigned char*)&hash1);
    uint160 hash2;
#if 0
    RIPEMD160((unsigned char*)&hash1, sizeof(hash1), (unsigned char*)&hash2);
#else
    uint32_t hash2_size = 0;
    EVP_Digest((unsigned char*)&hash1, sizeof(hash1),
               (unsigned char*)&hash2,
               &hash2_size,
               EVP_ripemd160(), nullptr);
#endif
    return hash2;
}

inline uint160 Hash160(const std::vector<unsigned char>& vch)
{
    return Hash160(vch.begin(), vch.end());
}

unsigned int MurmurHash3(unsigned int nHashSeed, const std::vector<unsigned char>& vDataToHash);

#endif
