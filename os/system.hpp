#ifndef _OS_SYSTEM_HPP
#define _OS_SYSTEM_HPP
#include "os/fwd.hpp"
#include "os/ref.hpp"
#include "gfx/fwd.hpp"

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

      void setDefaultWindow(WindowBase* window);
      /// Retorna un puntero a defaultWindow
      WindowBase* defaultWindow() const;
      /// Crea un puntero crudo envuelto en un puntero inteligente de un Surface
      SurfacePtr createRGBASurface(int width, int height) const;
      /// Carga un Surface a partir de una imagen
      SurfacePtr loadSurface(const char* filename) const;
      /// Obtiene el tamaño del área de trabajo del monitor más cercano a la ventana
      gfx::Size getWorkareaSizeFromWindow(WindowBase* window) const;

      /// Obtener la instancia del sistema
      /// @return Devuelve el puntero que apunta al sistema creado por create_system().
      ///         De lo contrario, devuelve nulo.
      static System* GetInstance();
   private:
      friend SystemPtr create_system();
      System();

      WindowBase* m_defaultWindow;
   };

   /// Crea un nuevo sistema. 
   /// 
   /// @return Una referencia al nuevo sistema creado.
   SystemPtr create_system();
} // namespace os
#endif