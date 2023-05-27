#include <DinrusPro/DinrusPro.h>

#if defined(CPU_X86) && !defined(COMPILER_MSC)
#include <cpuid.h>
#endif

#ifdef PLATFORM_FREEBSD
#include <sys/vmmeter.h>
#endif
#ifdef PLATFORM_MACOS
#include <mach/mach.h>
#include <mach/vm_statistics.h>
#endif

namespace ДинрусРНЦП {

#ifdef CPU_X86

static бул sHasMMX;
static бул sHasSSE;
static бул sHasSSE2;
static бул sHasSSE3;
static бул sHasAVX;
static бул sHypervisor;

static проц проверьПроцессор()
{
	static бул done;
	if(done) return;
	done = true;
	ONCELOCK {
		бцел eax, ebx, ecx, edx;
		#ifdef COMPILER_MSC
			цел cpuInfo[4];
			__cpuid(cpuInfo, 1);
			eax = cpuInfo[0];
			ebx = cpuInfo[1];
			ecx = cpuInfo[2];
			edx = cpuInfo[3];
		#else
			if(__get_cpuid(1, &eax, &ebx, &ecx, &edx))
		#endif
		// https://en.wikipedia.org/wiki/CPUID#EAX.3D1:_Processor_Info_and_Feature_Bits
			{
				sHasMMX = edx & (1 << 23);
				sHasSSE = edx & (1 << 25);
				sHasSSE2 = edx & (1 << 26);
				sHasSSE3 = ecx & 1;
				sHasAVX = ecx & (1 << 28);
				sHypervisor = ecx & (1 << 31);
			}
	}
}

ИНИЦБЛОК {
//	проверьПроцессор();
}

бул цпбММХ()        { проверьПроцессор(); return sHasMMX; }
бул цпбССЕ()        { проверьПроцессор(); return sHasSSE; }
бул цпбССЕ2()       { проверьПроцессор(); return sHasSSE2; }
бул цпбССЕ3()       { проверьПроцессор(); return sHasSSE3; }
бул цпбАВХ()        { проверьПроцессор(); return sHasAVX; }
бул цпбГипервизор() { проверьПроцессор(); return sHypervisor; }

#ifdef PLATFORM_POSIX
#ifdef PLATFORM_BSD
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <sys/sysinfo.h>
#endif
#endif

цел цпбЯдра()
{
	static цел n;
	ONCELOCK {
#ifdef PLATFORM_WIN32
#ifdef CPU_64
		бдол pa, sa;
		GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
		for(цел i = 0; i < 64; i++)
			n += !!(sa & ((бдол)1 << i));
#else
		DWORD pa, sa;
		GetProcessAffinityMask(GetCurrentProcess(), &pa, &sa);
		for(цел i = 0; i < 32; i++)
			n += !!(sa & (1 << i));
#endif
#elif defined(PLATFORM_POSIX)
#ifdef PLATFORM_BSD
		цел mib[2];
		т_мера len = sizeof(n);
		mib[0] = CTL_HW;
		mib[1] = HW_NCPU;
		sysctl(mib, 2, &n, &len, NULL, 0);
		n = минмакс(n, 1, 256);
#elif defined(PLATFORM_SOLARIS)
		n = минмакс((цел)sysconf(_SC_NPROCESSORS_ONLN), 1, 256);
#else
		n = минмакс(get_nprocs(), 1, 256);
#endif
#else
		n = 1;
#endif
	}
	return n;
}
#else

#ifdef PLATFORM_LINUX
	#ifdef PLATFORM_ANDROID
	#include <cpu-features.h>

	цел цпбЯдра()
	{
		return android_getCpuCount();
	}

	#else
	#include <sys/sysinfo.h>

	цел цпбЯдра()
	{
		return минмакс(get_nprocs(), 1, 256);
	}
	#endif
#else
цел цпбЯдра()
{
	return 1;
}
#endif

#endif

проц дайСтатусСисПамяти(бдол& total, бдол& available)
{
#ifdef PLATFORM_WIN32
	MEMORYSTATUSEX m;
	m.dwLength = sizeof(m);
	if(GlobalMemoryStatusEx(&m)) {
		total = m.ullTotalPhys;
		available = m.ullAvailPhys;
		return;
	}
#endif
#ifdef PLATFORM_LINUX
	цел pgsz = getpagesize();
	total = sysconf(_SC_PHYS_PAGES);
	available = sysconf(_SC_AVPHYS_PAGES);
	if(total >= 0 && available >= 0) {
		total *= pgsz;
		available *= pgsz;
		return;
	}
#endif
#ifdef PLATFORM_MACOS
	mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
	vm_statistics_data_t vmstat;

    цел mib[2];
    дол physical_memory;
    т_мера length;

    // Get the Physical memory size
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    length = sizeof(дол);

	if(host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmstat, &count) == KERN_SUCCESS &&
	   sysctl(mib, 2, &physical_memory, &length, NULL, 0) >= 0) {
		цел pgsz = getpagesize();
		available = (vmstat.free_count + vmstat.inactive_count) * pgsz;
//		available = physical_memory - (vmstat.wire_count + vmstat.inactive_count) * pgsz;
		total = physical_memory;
		return;
	}
#endif
#ifdef PLATFORM_FREEBSD
	u_int page_size;
    struct vmtotal vmt;
	т_мера vmt_size, uint_size;

    vmt_size = sizeof(vmt);
    uint_size = sizeof(page_size);

    if(sysctlbyname("vm.vmtotal", &vmt, &vmt_size, NULL, 0) >= 0 &&
       sysctlbyname("vm.stats.vm.v_page_size", &page_size, &uint_size, NULL, 0) >= 0) {
		available = vmt.t_free * (u_int64_t)page_size;
		total = vmt.t_avm * (u_int64_t)page_size;
		return;
    }
#endif
	total = 256*1024*1024;
	available = 16*1024*1024;
}

#define ЭНДИАН_РАЗВОРОТ { while(count--) { эндианРазворот(*v++); } }

проц эндианРазворот(бкрат *v, т_мера count) ЭНДИАН_РАЗВОРОТ
проц эндианРазворот(крат *v, т_мера count) ЭНДИАН_РАЗВОРОТ
проц эндианРазворот(бцел *v, т_мера count) ЭНДИАН_РАЗВОРОТ
проц эндианРазворот(цел *v, т_мера count) ЭНДИАН_РАЗВОРОТ
проц эндианРазворот(дол *v, т_мера count) ЭНДИАН_РАЗВОРОТ
проц эндианРазворот(бдол *v, т_мера count) ЭНДИАН_РАЗВОРОТ

}