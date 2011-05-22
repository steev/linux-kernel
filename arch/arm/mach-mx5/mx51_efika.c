/*
 * Copyright © 2011 Saleem Abdulrasool <compnerd@compnerd.org>
 * vim: set ft=c.doxygen noet sts=8 sw=8 tw=80 :
 */

static __init void mx51_efika_fixup(struct machine_desc *desc, struct tag *tags,
				    char **cmdline, struct meminfo *meminfo)
{
}

static __init void mx51_efika_init_machine(void)
{
}

MACHINE_START(MX51_EFIKA, "Genesi EfikaMX")
	/* Maintainer: Genesi USA, Inc. */
	.fixup        = mx51_efika_fixup,
	.init_machine = mx51_efika_init_machine,
MACHINE_END

