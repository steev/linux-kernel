/*
 * Copyright © 2011 Saleem Abdulrasool <compnerd@compnerd.org>
 * vim: set ft=c.doxygen noet sts=8 sw=8 tw=80 :
 */

#define MEM_OPT						("mem=")
#define GPU_MEM_OPT					("gpu_memory=")

#define EFIKA_DEFAULT_FB_MEM				(SZ_32M)
#define EFIKA_DEFAULT_GPU_MEM				(SZ_64M)


static int num_cpu_wp;
static struct cpu_wp cpu_wp_auto[] = {
	{
		.pll_rate    = 1000000000,
		.cpu_rate    = 1000000000,
		.pdf         = 0,
		.mfi         = 10,
		.mfd         = 11,
		.mfn         = 5,
		.cpu_podf    = 0,
		.cpu_voltage = 1175000,
	},
	{
		.pll_rate    = 800000000,
		.cpu_rate    = 800000000,
		.pdf         = 0,
		.mfi         = 8,
		.mfd         = 2,
		.mfn         = 1,
		.cpu_podf    = 0,
		.cpu_voltage = 1100000,
	},
	{
		.pll_rate    = 800000000,
		.cpu_rate    = 400000000,
		.pdf         = 0,
		.mfi         = 8,
		.mfd         = 2,
		.mfn         = 1,
		.cpu_podf    = 1,
		.cpu_voltage = 950000,
	},
	{
		.pll_rate    = 800000000,
		.cpu_rate    = 166250000,
		.pdf         = 0,
		.mfi         = 8,
		.mfd         = 2,
		.mfn         = 1,
		.cpu_podf    = 4,
		.cpu_voltage = 850000,
	},
};

static struct cpu_wp * mx51_efika_get_cpu_wp(int *wp)
{
	*wp = num_cpu_wp;
	return cpu_wp_auto;
}

static void mx51_efika_set_num_cpu_wp(int num)
{
	num_cpu_wp = num;
}

static __init void mx51_efika_display_adjust_mem(int gpu_start, int gpu_mem,
						 int fb_mem)
{
}

static __init void mx51_efika_fixup(struct machine_desc *desc, struct tag *tags,
				    char **cmdline, struct meminfo *meminfo)
{
	char *s;
	char *str;
	struct tag *tag;
	struct tag *cmd_tag;
	struct tag *mem_tag;
	int fb_mem = EFIKA_DEFAULT_FB_MEM;
	int gpu_mem = EFIKA_DEFAULT_GPU_MEM;
	int total_mem;
	int avail_mem;

	mxc_set_cpu_type(MXC_CPU_MX51);

	get_cpu_wp     = mx51_efika_get_cpu_wp;
	set_num_cpu_wp = mx51_efika_set_num_cpu_wp;

	for_each_tag(tag, tags) {
		switch (tag->hdr.tag) {
		case ATAG_MEM:
			mem_tag = tag;
			break;
		case ATAG_CMDLINE:
			cmd_tag = tag;
			break;
		}
	}

	if (mem_tag) {
		total_mem = mem_tag->u.mem.size;
		avail_mem = total_mem - gpu_mem - fb_mem;

		if (cmd_tag) {
			str = cmd_tag->u.cmdline.cmdline;

			/* mem= command line option */
			if ((s = strstr(str, MEM_OPT)) != NULL) {
				avail_mem = memparse(s + strlen(MEM_OPT), &s);
				if (avail_mem == 0 || avail_mem > total_mem)
					avail_mem = total_mem - gpu_mem - fb_mem;
			}

			/* gpu_memory= command line option */
			if ((s = strstr(str, GPU_MEM_OPT)) != NULL)
				gpu_mem = memparse(s + strlen(GPU_MEM_OPT), &s);

			fb_mem = total_mem - avail_mem - gpu_mem;
			if (fb_mem < 0) {
				gpu_mem = total_mem - avail_mem;
				fb_mem = 0;
			}
		}

		mem_tag->u.mem.size = avail_mem;

		/* reserve graphics memory */
		gpu_device.resource[5].start = mem_tag->u.mem.start + avail_mem;
		gpu_device.resource[5].end   = gpu_device.resource[5].start + gpu_mem - 1;

		if (fb_mem) {
			mxcfb_resources[0].start = gpu_device.resource[5].end + 1;
			mxcfb_resources[0].end   = mxcfb_resources[0].start + fb_mem - 1;
		} else {
			mxcfb_resources[0].start = 0;
			mxcfb_resources[0].end   = 0;
		}
	}
}

static __init void mx51_efika_init_machine(void)
{
	mxc_cpu_common_init();
}

MACHINE_START(MX51_EFIKA, "Genesi EfikaMX")
	/* Maintainer: Genesi USA, Inc. */
	.fixup        = mx51_efika_fixup,
	.init_machine = mx51_efika_init_machine,
MACHINE_END

