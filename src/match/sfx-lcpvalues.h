/*
  Copyright (c) 2007 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2007 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef SFX_LCPVALUES_H
#define SFX_LCPVALUES_H
#include "core/error_api.h"
#include "core/codetype.h"
#include "core/unused_api.h"
#include "core/encseq.h"
#include "lcpoverflow.h"
#include "bcktab.h"
#include "sfx-suffixgetset.h"

typedef struct Outlcpinfo Outlcpinfo;

typedef struct
{
  unsigned long *isset;
  unsigned long *bucketoflcpvalues,
                numofentries,
                numoflargelcpvalues,
                subbucketleft;
} GtLcpvalues;

/*@unused@*/ static inline void lcptab_update(GtLcpvalues *tableoflcpvalues,
                                              unsigned long idx,
                                              unsigned long value)
{
  gt_assert (tableoflcpvalues != NULL &&
             tableoflcpvalues->bucketoflcpvalues != NULL &&
             tableoflcpvalues->subbucketleft+idx <
             tableoflcpvalues->numofentries);
  GT_SETIBIT(tableoflcpvalues->isset,tableoflcpvalues->subbucketleft+idx);
  tableoflcpvalues->bucketoflcpvalues[tableoflcpvalues->subbucketleft+idx]
    = value;
  if (value >= (unsigned long) LCPOVERFLOW)
  {
    tableoflcpvalues->numoflargelcpvalues++; /* this may overcount as there may
                                                be some value at index <idx>
                                                which was already overflowing */
  }
}

/*@unused@*/ static inline unsigned long lcpsubtab_getvalue(
                                        const GtLcpvalues *tableoflcpvalues,
                                        unsigned long idx)
{
  gt_assert (tableoflcpvalues != NULL &&
             tableoflcpvalues->bucketoflcpvalues != NULL &&
             tableoflcpvalues->subbucketleft+idx <
             tableoflcpvalues->numofentries);
  gt_assert(GT_ISIBITSET(tableoflcpvalues->isset,
                         tableoflcpvalues->subbucketleft+idx));
  return tableoflcpvalues->bucketoflcpvalues
                           [tableoflcpvalues->subbucketleft+idx];
}

Outlcpinfo *gt_Outlcpinfo_new(const char *indexname,
                              unsigned int numofchars,
                              unsigned int prefixlength,
                              unsigned long totallength,
                              GtError *err);

void gt_Outlcpinfo_reinit(Outlcpinfo *outlcpinfo,
                          unsigned int numofchars,
                          unsigned int prefixlength,
                          unsigned long numoflcpvalues);

void gt_Outlcpinfo_delete(Outlcpinfo *outlcpinfo);

unsigned long gt_Outlcpinfo_numoflargelcpvalues(const Outlcpinfo *outlcpinfo);

unsigned long gt_Outlcpinfo_maxbranchdepth(const Outlcpinfo *outlcpinfo);

void gt_Outlcpinfo_prebucket(Outlcpinfo *outlcpinfo,
                             GtCodetype code,
                             unsigned long subbucketleft);

void gt_Outlcpinfo_nonspecialsbucket(Outlcpinfo *outlcpinfo,
                                     unsigned int prefixlength,
                                     GT_UNUSED GtSuffixsortspace *sssp,
                                     GtLcpvalues *tableoflcpvalues,
                                     const Bucketspecification *bucketspec,
                                     GtCodetype code);

void gt_Outlcpinfo_postbucket(Outlcpinfo *outlcpinfo,
                              unsigned int prefixlength,
                              GtSuffixsortspace *sssp,
                              const Bcktab *bcktab,
                              const Bucketspecification *bucketspec,
                              GtCodetype code);

GtLcpvalues *gt_Outlcpinfo_resizereservoir(Outlcpinfo *outlcpinfo,
                                           const Bcktab *bcktab);

void gt_Outlcpinfo_check_lcpvalues(const GtEncseq *encseq,
                                   GtReadmode readmode,
                                   const GtSuffixsortspace *sortedsample,
                                   unsigned long effectivesamplesize,
                                   unsigned long maxdepth,
                                   GT_UNUSED const Outlcpinfo
                                     *outlcpinfosample);

#endif
