#include <windows.h>

class base::Mutex::MutexImpl
{
public:
   MutexImpl() {
      InitializeCriticalSection(&m_criticalSection);
   }

   ~MutexImpl() {
      DeleteCriticalSection(&m_criticalSection);
   }

   void lock() {
      EnterCriticalSection(&m_criticalSection);
   }

   bool try_lock() {
      return TryEnterCriticalSection(&m_criticalSection) ? true : false;
   }

   void unlock() {
      LeaveCriticalSection(&m_criticalSection);
   }
private:
   CRITICAL_SECTION m_criticalSection;
};