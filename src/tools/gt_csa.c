/*
  Copyright (c) 2005-2008 Gordon Gremme <gremme@zbh.uni-hamburg.de>
  Copyright (c) 2005-2008 Center for Bioinformatics, University of Hamburg

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

#include "core/ma.h"
#include "core/option.h"
#include "core/outputfile.h"
#include "core/unused_api.h"
#include "extended/csa_stream.h"
#include "extended/genome_node.h"
#include "extended/gff3_in_stream.h"
#include "extended/gff3_out_stream.h"
#include "tools/gt_csa.h"

typedef struct {
  bool verbose;
  unsigned long join_length;
  GtOutputFileInfo *ofi;
  GtGenFile *outfp;
} CSAArguments;

static void* gt_csa_arguments_new(void)
{
  CSAArguments *arguments = gt_calloc(1, sizeof *arguments);
  arguments->ofi = gt_outputfileinfo_new();
  return arguments;
}

static void gt_csa_arguments_delete(void *tool_arguments)
{
  CSAArguments *arguments = tool_arguments;
  if (!arguments) return;
  gt_genfile_close(arguments->outfp);
  gt_outputfileinfo_delete(arguments->ofi);
  gt_free(arguments);
}

static GtOptionParser* gt_csa_option_parser_new(void *tool_arguments)
{
  CSAArguments *arguments = tool_arguments;
  GtOptionParser *op;
  GtOption *option;
  assert(arguments);

  /* init */
  op = gt_option_parser_new("[option ...] [GFF3_file]",
                         "Replace spliced alignments with computed consensus "
                         "spliced alignments.");

  /* -join-length */
  option = gt_option_new_ulong("join-length", "set join length for the spliced "
                            "alignment clustering", &arguments->join_length,
                            DEFAULT_JOIN_LENGTH);
  gt_option_parser_add_option(op, option);

  /* -v */
  option = gt_option_new_verbose(&arguments->verbose);
  gt_option_parser_add_option(op, option);

  /* output file options */
  gt_outputfile_register_options(op, &arguments->outfp, arguments->ofi);

  gt_option_parser_set_max_args(op, 1);

  return op;
}

static int gt_csa_runner(GT_UNUSED int argc, const char **argv, int parsed_args,
                         void *tool_arguments, GtError *err)
{
  GtNodeStream *gff3_in_stream,
               *csa_stream,
               *gff3_out_stream;
  GtGenomeNode *gn;
  CSAArguments *arguments = tool_arguments;
  int had_err;

  gt_error_check(err);
  assert(arguments);

  /* create the streams */
  gff3_in_stream  = gt_gff3_in_stream_new_sorted(argv[parsed_args]);
  if (arguments->verbose && arguments->outfp)
    gt_gff3_in_stream_show_progress_bar((GtGFF3InStream*) gff3_in_stream);
  csa_stream      = gt_csa_stream_new(gff3_in_stream, arguments->join_length);
  gff3_out_stream = gt_gff3_out_stream_new(csa_stream, arguments->outfp);

  /* pull the features through the stream and free them afterwards */
  while (!(had_err = gt_node_stream_next(gff3_out_stream, &gn, err)) &&
         gn) {
    gt_genome_node_rec_delete(gn);
  }

  /* free */
  gt_node_stream_delete(gff3_out_stream);
  gt_node_stream_delete(csa_stream);
  gt_node_stream_delete(gff3_in_stream);

  return had_err;
}

GtTool* gt_csa(void)
{
  return gt_tool_new(gt_csa_arguments_new,
                  gt_csa_arguments_delete,
                  gt_csa_option_parser_new,
                  NULL,
                  gt_csa_runner);
}
