#pragma once

#include "base58.h"

std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend)
{
  CAutoBN_CTX pctx;
  CBigNum bn58 = 58;
  CBigNum bn0 = 0;

  // Convert big endian data to little endian
  // Extra zero at the end make sure bignum will interpret as a positive number
  std::vector<unsigned char> vchTmp(pend-pbegin+1, 0);
  reverse_copy(pbegin, pend, vchTmp.begin());

  // Convert little endian data to bignum
  CBigNum bn;
  bn.setvch(vchTmp);

  // Convert bignum to std::string
  std::string str;
  // Expected size increase from base58 conversion is approximately 137%
  // use 138% to be safe
  str.reserve((pend - pbegin) * 138 / 100 + 1);
  CBigNum dv;
  CBigNum rem;
  while (bn > bn0)
  {
    if (!BN_div(dv.GetBIGNUM(), rem.GetBIGNUM(), bn.GetBIGNUM(), bn58.GetBIGNUM(), pctx))
      throw bignum_error("EncodeBase58 : BN_div failed");
    bn = dv;
    unsigned int c = rem.getulong();
    str += pszBase58[c];
  }

  // Leading zeroes encoded as base58 zeros
  for (const unsigned char* p = pbegin; p < pend && *p == 0; p++)
    str += pszBase58[0];

  // Convert little endian std::string to big endian
  reverse(str.begin(), str.end());
  return str;
}