/*
  Copyright (c) 2011-2012 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2011-2012 Center for Bioinformatics, University of Hamburg

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

#ifndef FIRSTCODES_CACHE_H
#define FIRSTCODES_CACHE_H

#include "firstcodes-spacelog.h"

typedef struct GtArrayGtIndexwithcode GtArrayGtIndexwithcode;

GtArrayGtIndexwithcode *gt_firstcodes_binsearchcache_new(
                                      const unsigned long *allfirstcodes,
                                      unsigned long differentcodes,
                                      unsigned int addbscache_depth,
                                      GtFirstcodesspacelog *fcsl);

void gt_firstcodes_binsearchcache_delete(GtArrayGtIndexwithcode *binsearchcache,
                                         GtFirstcodesspacelog *fcsl);

unsigned long gt_firstcodes_binsearchcache_allfirstcodes0(
                    const GtArrayGtIndexwithcode *binsearchcache);

const unsigned long *gt_firstcodes_find_accu(
                                 unsigned long *foundcode,
                                 const unsigned long *allfirstcodes,
                                 unsigned long differentcodes,
                                 const GtArrayGtIndexwithcode *binsearchcache,
                                 unsigned long code);

#endif
