/**
 * traceback for cpp
 *
 *  Created on: 2018-01-08
 *      Author: owent
 *
 *  @note Required flag -rdynamic or addr2line to get the function name when using gcc/clang in unix like system
 *  @note Using addr2line -Cfpe <exe_path> [func_addr...] for more detail when using gcc/clang
 *  @note Required flag /DEBUG /Zi to generate .pdb file when using MSVC
 */

#include <cstdlib>
#include <cstdio>
#include <string>

#include <string.h>
#include <iostream>
using namespace std;


struct A{
    A() {
        m_a = 0;
        memset(m_v1, 0, sizeof(m_v1));
        memset(m_v2, 0, sizeof(m_v2));
        memset(m_v3, 0, sizeof(m_v3));
        memset(m_v4, 0, sizeof(m_v4));
        memset(m_v5, 0, sizeof(m_v5));
        memset(m_v6, 0, sizeof(m_v6));
        memset(m_v7, 0, sizeof(m_v7));
        memset(m_v8, 0, sizeof(m_v8));
        memset(m_v9, 0, sizeof(m_v9));
        memset(m_v10, 0, sizeof(m_v10));
        memset(m_v11, 0, sizeof(m_v11));
        memset(m_v12, 0, sizeof(m_v12));
        memset(m_v13, 0, sizeof(m_v13));
        memset(m_v14, 0, sizeof(m_v14));
        memset(m_v15, 0, sizeof(m_v15));
        memset(m_v16, 0, sizeof(m_v16));
        memset(m_v17, 0, sizeof(m_v17));
        memset(m_v18, 0, sizeof(m_v18));
        memset(m_v19, 0, sizeof(m_v19));
        memset(m_v19, 0, sizeof(m_v19));
        memset(m_v20, 0, sizeof(m_v20));
        memset(m_v21, 0, sizeof(m_v21));

    }
    int m_a;
    char m_v1[50];
    char m_v2[50];
    char m_v3[50];
    char m_v4[50];
    char m_v5[50];
    char m_v6[50];
    char m_v7[200];
    char m_v8[200];
    char m_v9[200];
    char m_v10[200];
    char m_v11[200];
    char m_v12[1000];
    char m_v13[1000];
    char m_v14[30];
    char m_v15[30];
    char m_v16[30];
    char m_v17[30];
    char m_v18[30];
    char m_v19[20];
    char m_v20[20];
    char m_v21[20];
    char m_v22[20];
    char m_v23[20];
    char m_v24[20];
    char m_v25[20];
    char m_v26[20];
    char m_v27[100];
    char m_v28[100];
    char m_v29[100];
    char m_v30[100];
    char m_v31[100];

    bool operator<(const A& obj) {
        if (m_a != obj.m_a) {
            return m_a > obj.m_a;
        }
        int nv1 = memcmp(m_v1, obj.m_v1, sizeof(m_v1));
        if (nv1!=0) {
            return nv1>0;
        }
        int nv2 = memcmp(m_v2, obj.m_v2, sizeof(m_v2));
        if (nv2!=0) {
            return nv2>0;
        }
        int nv3 = memcmp(m_v3, obj.m_v3, sizeof(m_v3));
        if (nv3!=0) {
            return nv3>0;
        }
        int nv4 = memcmp(m_v4, obj.m_v4, sizeof(m_v4));
        if (nv4!=0) {
            return nv4>0;
        }
        int nv5 = memcmp(m_v5, obj.m_v5, sizeof(m_v5));
        if (nv5!=0) {
            return nv5>0;
        }
        int nv6 = memcmp(m_v6, obj.m_v6, sizeof(m_v6));
        if (nv6!=0) {
            return nv6>0;
        }
        int nv7 = memcmp(m_v7, obj.m_v7, sizeof(m_v7));
        if (nv7!=0) {
            return nv7>0;
        }
        int nv8 = memcmp(m_v8, obj.m_v8, sizeof(m_v8));
        if (nv8!=0) {
            return nv8>0;
        }
        int nv9 = memcmp(m_v9, obj.m_v9, sizeof(m_v9));
        if (nv9!=0) {
            return nv9>0;
        }
        int nv10 = memcmp(m_v10, obj.m_v10, sizeof(m_v10));
        if (nv10!=0) {
            return nv10>0;
        }
        int nv11 = memcmp(m_v11, obj.m_v11, sizeof(m_v11));
        if (nv11!=0) {
            return nv11>0;
        }
        int nv12 = memcmp(m_v12, obj.m_v12, sizeof(m_v12));
        if (nv12!=0) {
            return nv12>0;
        }
        int nv13 = memcmp(m_v13, obj.m_v13, sizeof(m_v13));
        if (nv13!=0) {
            return nv13>0;
        }
        int nv14 = memcmp(m_v14, obj.m_v14, sizeof(m_v14));
        if (nv14!=0) {
            return nv14>0;
        }
        int nv15 = memcmp(m_v15, obj.m_v15, sizeof(m_v15));
        if (nv15!=0) {
            return nv15>0;
        }
        int nv16 = memcmp(m_v16, obj.m_v16, sizeof(m_v16));
        if (nv16!=0) {
            return nv16>0;
        }
        int nv17 = memcmp(m_v17, obj.m_v17, sizeof(m_v17));
        if (nv17!=0) {
            return nv17>0;
        }
        int nv18 = memcmp(m_v18, obj.m_v18, sizeof(m_v18));
        if (nv18!=0) {
            return nv18>0;
        }
        int nv19 = memcmp(m_v19, obj.m_v19, sizeof(m_v19));
        if (nv19!=0) {
            return nv19>0;
        }
        int nv20 = memcmp(m_v20, obj.m_v20, sizeof(m_v20));
        if (nv20!=0) {
            return nv20>0;
        }
        int nv21 = memcmp(m_v21, obj.m_v21, sizeof(m_v21));
        if (nv21!=0) {
            return nv21>0;
        }
    }

    free(symbol);
    SymCleanup(process);
    #endif

    // ================== Using IDebugClient ==================
    #if defined(USING_WIN_DBG_ENG_COM) && USING_WIN_DBG_ENG_COM
    print_trace_com_holder<IDebugClient> dbg_cli;
    print_trace_com_holder<IDebugControl> dbg_ctrl;
    print_trace_com_holder<IDebugSymbols> dbg_sym;
    if (S_OK != ::DebugCreate(__uuidof(IDebugClient), dbg_cli.to_pvoid_ptr())) {
        return;
    }
    const bool res0 = (S_OK == dbg_cli->QueryInterface(
        __uuidof(IDebugControl),
        dbg_ctrl.to_pvoid_ptr()
    ));
    if (!res0) {
        return;
    }

    const bool res1 = (S_OK == dbg_cli->AttachProcess(
        0,
        ::GetCurrentProcessId(),
        DEBUG_ATTACH_NONINVASIVE | DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND
    ));
    if (!res1) {
        return;
    }

    if (S_OK != dbg_ctrl->WaitForEvent(DEBUG_WAIT_DEFAULT, INFINITE)) {
        return;
    }

    // No cheking: QueryInterface sets the output parameter to NULL in case of error.
    dbg_cli->QueryInterface(__uuidof(IDebugSymbols), dbg_sym.to_pvoid_ptr());

    for(unsigned short i = 0; i < frames; i++ ) {
        if (!dbg_sym.is_inited()) {
            break;
        }

        const ULONG64 offset = reinterpret_cast<ULONG64>(stack[i]);

        char name[256];
        name[0] = '\0';
        ULONG size = 0;
        bool res = (S_OK == dbg_sym->GetNameByOffset(
            offset,
            name,
            sizeof(name),
            &size,
            0
        ));

        std::string result;
        if (!res && size != 0) {
            result.resize(size);
            res = (S_OK == dbg_sym->GetNameByOffset(
                offset,
                &result[0],
                static_cast<ULONG>(result.size()),
                &size,
                0
            ));
        } else if (res) {
            result = name;
        }

        printf( "Frame %02i: (%s) [0x%p]\n", i, result.c_str(), stack[i]);
    }
    #endif
#endif
}

#else

#if defined(USING_GNU_EXECINFO) && USING_GNU_EXECINFO
#include <execinfo.h>
#endif

#if defined(USING_GNU_UNWIND) && USING_GNU_UNWIND
#include <unwind.h>

struct print_trace_unwind_state_t {
    size_t frames_to_skip;
    _Unwind_Word* current;
    _Unwind_Word* end;
};

static _Unwind_Reason_Code print_trace_unwind_callback(::_Unwind_Context* context, void* arg) {
    // Note: do not write `::_Unwind_GetIP` because it is a macro on some platforms.
    // Use `_Unwind_GetIP` instead!
    print_trace_unwind_state_t* const state = reinterpret_cast<print_trace_unwind_state_t*>(arg);
    if (state->frames_to_skip) {
        --state->frames_to_skip;
        return _Unwind_GetIP(context) ? ::_URC_NO_REASON : ::_URC_END_OF_STACK;
    }

    *state->current = _Unwind_GetIP(context);

    ++state->current;
    if (!*(state->current - 1) || state->current == state->end) {
        return ::_URC_END_OF_STACK;
    }

    return ::_URC_NO_REASON;
}

#endif

/* Obtain a backtrace and print it to stdout. */

struct stacktrace_symbol_group_t {
    std::string module_name;
    std::string func_name;
    std::string func_offset;
    std::string func_address;
};

inline bool stacktrace_is_space_char(char c) USING_CXX11_NOEXCEPT { return ' ' == c || '\r' == c || '\t' == c || '\n' == c; }

static const char *stacktrace_skip_space(const char *name) USING_CXX11_NOEXCEPT {
    if (NULL == name) {
        return name;
    }

    while (*name && stacktrace_is_space_char(*name)) {
        ++name;
    }

    return name;
}

inline bool stacktrace_is_number_char(char c) USING_CXX11_NOEXCEPT { return c >= '0' && c <= '9'; }

inline bool stacktrace_is_ident_char(char c) USING_CXX11_NOEXCEPT {
    return '_' == c || '$' == c || stacktrace_is_number_char(c) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
           (c & 0x80); // utf-8 or unicode
}

static const char *stacktrace_get_ident_end(const char *name) USING_CXX11_NOEXCEPT {
    if (NULL == name) {
        return name;
    }

    while (*name && !stacktrace_is_space_char(*name) && '(' != *name && ')' != *name && '+' != *name) {
        ++name;
    }

    return name;
}

static bool stacktrace_pick_ident(const char *name, const char *&start, const char *&end, char &previous_c,
                                  bool &clear_sym) USING_CXX11_NOEXCEPT {
    previous_c = 0;
    start = name;
    end = name;

    bool ret = false;
    if (NULL == name) {
        return false;
    }

    while (*name) {
        // gcc in linux will get a string like MODULE_NAME(FUNCTION_NAME+OFFSET) [HEX_ADDRESS]
        // if we meet a (, we should clear the function name cache
        if ('(' == *name) {
            clear_sym = true;
        }

        name = stacktrace_skip_space(name);

        if (stacktrace_is_ident_char(*name)) {
            start = name;
            end = stacktrace_get_ident_end(name);
            ret = true;
            break;
        } else {
            previous_c = *name;
            ++name;
        }
    }

    return ret;
}

static void stacktrace_fix_number(std::string &num) USING_CXX11_NOEXCEPT {
    size_t fixed_len = num.size();
    while (fixed_len > 0 && (num[fixed_len - 1] > '9' || num[fixed_len - 1] < '0')) {
        --fixed_len;
    }

    num.resize(fixed_len);
}

static void stacktrace_pick_symbol_info(const char *name, stacktrace_symbol_group_t &out) {
    out.module_name.clear();
    out.func_name.clear();
    out.func_offset.clear();
    out.func_address.clear();

    if (NULL == name || 0 == *name) {
        return;
    }

    name = stacktrace_skip_space(name);
    while (name) {
        const char *start;
        char previous_c;
        bool clear_sym = false;

        if (stacktrace_pick_ident(name, start, name, previous_c, clear_sym)) {
            if (stacktrace_is_number_char(*start)) {
                if (clear_sym) {
                    out.func_name.clear();
                }

                if ('+' == previous_c) {
                    out.func_offset = "+";
                    out.func_offset.insert(out.func_offset.end(), start, name);
                    stacktrace_fix_number(out.func_offset);
                } else {
                    out.func_address.assign(start, name);
                    stacktrace_fix_number(out.func_address);
                }
            } else {
                if (out.module_name.empty()) {
                    out.module_name.assign(start, name);
                } else {
                    out.func_name.assign(start, name);
                }
            }
        } else {
            if (clear_sym) {
                out.func_name.clear();
            }
            break;
        }

        name = stacktrace_skip_space(name);
    }
}


void print_trace () {
#if defined(USING_GNU_EXECINFO) && USING_GNU_EXECINFO
    {
        void *array[BACKTRACE_MAX_FRAME_NUMBER];
        size_t size;
        char **func_name_cache;

        size = backtrace (array, BACKTRACE_MAX_FRAME_NUMBER);
        func_name_cache = backtrace_symbols (array, size);

        printf("print stacktrace using execinfo.h with %d stacks\n",(int)size);
        for (size_t i = 0; i < size; i++) {
            stacktrace_symbol_group_t symbol;
            printf("func_name :%s\n", func_name_cache[i]);
            stacktrace_pick_symbol_info(func_name_cache[i], symbol);

#if defined(USING_LIBSTDCXX_ABI) || defined(USING_LIBCXX_ABI)
            if (!symbol.func_name.empty()) {
                int cxx_abi_status;
                char* realfunc_name = abi::__cxa_demangle(symbol.func_name.c_str(), 0, 0, &cxx_abi_status);
                if (NULL != realfunc_name) {
                    symbol.func_name = realfunc_name;
                }

                if (NULL != realfunc_name) {
                    free(realfunc_name);
                }
            }
#endif
            printf("Frame #%02d: (%s%s) [%s]\n", static_cast<int>(i), symbol.func_name.c_str(), symbol.func_offset.c_str(), symbol.func_address.c_str());
        }

        free (func_name_cache);
    }
#endif

#if defined(USING_GNU_UNWIND) && USING_GNU_UNWIND
    {
        _Unwind_Word stacks[BACKTRACE_MAX_FRAME_NUMBER];
        print_trace_unwind_state_t state;
        state.frames_to_skip = 0;
        state.current = stacks;
        state.end = stacks + BACKTRACE_MAX_FRAME_NUMBER;

        ::_Unwind_Backtrace(&print_trace_unwind_callback, &state);
        size_t frames_count = state.current - &stacks[0];

        printf("print stacktrace using unwind.h with %d stacks\n",(int)frames_count);
        for (size_t i = 0; i < frames_count; ++ i) {
            printf("Frame #%02d: () [0x%lx]\n", static_cast<int>(i), stacks[i]);
        }
    }
#endif
}
#endif

#endif

void func1(int times) {
    if (times > 0) {
        func1(times - 1);
        return;
    }

    print_trace();
}

class functor2 {
public:
    void func2(int times) {
        if (times & 0x01) {
            func2(times - 1);
        } else {
            func1(times - 1);
        }
    }
};




struct B{
    struct {int a;} BA;
};

static void func8(){
    print_trace();
}

void func7()
{
    func8();
}



int main (int argc, char* argv[]) {
    A a;
    A b;
    char v2[4000] = {0};
    char v1[4000] = {0};
    time_t time1 = time(NULL);

    for (int i=1; i<100001000; i++) {
        //bool l = memcpy(v1, v2, 4000);
        bool l = a<b;
    }
    time_t time2 = time(NULL);
    cout << time2 - time1 << endl;
}
