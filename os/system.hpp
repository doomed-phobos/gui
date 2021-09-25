#ifndef _OS_SYSTEM_HPP
#define _OS_SYSTEM_HPP
#include "os/fwd.hpp"
#include "os/ref.hpp"

namespace os
{
   class System : public Referenceable
   {
   public:
      ~System();

      // ============
      // EVENTOS
      // ============
      void waitEvents();
      void pollEvents();

      void setDefaultWindow(Window* window);
      /// Retorna un puntero a defaultWindow
      Window* defaultWindow() const;

      /// Crea un puntero crudo envuelto en un puntero inteligente de un Surface
      SurfacePtr createRGBASurface(int width, int height);
      /// Carga una imagen como Surface
      SurfacePtr loadSurface(const char* filename);

      /// Obtener la instancia del sistema
      /// @return Devuelve el puntero que apunta al sistema creado por create_system().
      ///         De lo contrario, devuelve nulo.
      static System* GetInstance();
   private:
      friend SystemPtr create_system();
      System();

      Window* m_defaultWindow;
   };

   /// Crea un nuevo sistema. 
   /// 
   /// @return Una referencia al nuevo sistema creado.
   SystemPtr create_system();
} // namespace os
#endif