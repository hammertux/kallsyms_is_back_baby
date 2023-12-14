#include <linux/module.h>
#include <linux/kprobes.h>


MODULE_AUTHOR("hammertux");
MODULE_DESCRIPTION("kallsyms_lookup_name is back mofos ;)");
MODULE_LICENSE("GPL");

static struct kprobe kp = { 
    .symbol_name = "kallsyms_lookup_name",
};

typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
kallsyms_lookup_name_t ksyms_lookup;



static __init int init_mod(void)
{
    pr_info("Initializing kallsyms_hack module\n");
    if(!(register_kprobe(&kp))) {
        ksyms_lookup = (kallsyms_lookup_name_t) kp.addr;   
    }

    unregister_kprobe(&kp);

    //use ksyms_lookup instead of kallsyms_lookup_name

    pr_info("Accessing a non-exported symbol (e.g., '_stext' - start of kernel text segment in memory) = 0x%lx\n", ksyms_lookup("_stext"));
    pr_info("To check: run 'sudo cat /proc/kallsyms | grep _stext'\n");

    return 0;
}


static __exit void exit_mod(void)
{
    pr_info("kallsyms_hack: bye bye!\n");
}


module_init(init_mod);
module_exit(exit_mod);