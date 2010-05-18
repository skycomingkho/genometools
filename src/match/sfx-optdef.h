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

#ifndef SFX_OPTDEF_H
#define SFX_OPTDEF_H

#include <stdbool.h>
#include "core/str.h"
#include "core/str_array.h"
#include "core/readmode.h"
#include "sfx-strategy.h"
#include "eis-bwtseq-param.h"

#define PREFIXLENGTH_AUTOMATIC 0
#define MAXDEPTH_AUTOMATIC     0

typedef struct
{
  GtStr *indexname,
        *smap,
        *sat;
  GtStrArray *filenametab;
  bool isdna,
       isprotein,
       isplain,
       outtistab,
       outdestab,
       outsdstab,
       outssptab;
} Filenames2encseqoptions;

typedef struct
{
  unsigned int numofparts,
               prefixlength;
  GtStr *inputindex,
        *maxdepth;
  GtOption *optionalgboundsref;
  GtStrArray *algbounds;
  GtReadmode readmode;
  bool beverbose,
       showtime,
       showprogress,
       outsuftab,
       outlcptab,
       outbwttab,
       outbcktab,
       outkystab,
       outkyssort,
       iteratorbasedkmerscanning;
  GtStr *optionkysargumentstring;
  Sfxstrategy sfxstrategy;
  Filenames2encseqoptions fn2encopt;
  struct bwtOptions bwtIdxParams;
} Suffixeratoroptions;

#endif
