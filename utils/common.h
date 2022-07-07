#pragma once

inline void SpinlockPause(void) {
#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
  __asm__ __volatile__("rep; nop" : : );  // 会翻译成pause指令, 参照 https://www.felixcloutier.com/x86/pause.html
#endif
}