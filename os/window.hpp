#ifndef _OS_WINDOW_HPP
#define _OS_WINDOW_HPP

#ifdef CORE_WINDOWS
#  include "os/win/win_window.hpp"

namespace os {typedef priv::WinWindow Window;}
#else
#  error "Unsupported cross-platform window"
#endif

#endif