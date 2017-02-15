/**
 * This file is part of Tales of Berseria "Fix".
 *
 * Tales of Berseria "Fix" is free software : you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by The Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Tales of Berseria "Fix" is distributed in the hope that it will be
 * useful,
 *
 * But WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tales of Berseria "Fix".
 *
 *   If not, see <http://www.gnu.org/licenses/>.
 *
**/

#ifndef __TBF__RENDER_H__
#define __TBF__RENDER_H__
#define NOMINMAX

#include "command.h"

#include <d3d9.h>
#include <d3d9types.h>

#include <Windows.h>

#include <unordered_set>

namespace tbf
{
  namespace RenderFix
  {
    void Init     (void);
    void Shutdown (void);

    void Reset    ( IDirect3DDevice9      *This,
                    D3DPRESENT_PARAMETERS *pPresentationParameters );

    void TriggerReset (void);

    // Indicates whether setting changes need a device reset
    struct reset_state_s {
      bool textures = false;
      bool graphics = false;
    } extern need_reset;

    class CommandProcessor : public SK_IVariableListener {
    public:
      CommandProcessor (void);

      virtual bool OnVarChange (SK_IVariable* var, void* val = NULL);

      static CommandProcessor* getInstance (void)
      {
        if (pCommProc == NULL)
          pCommProc = new CommandProcessor ();

        return pCommProc;
      }

    protected:
      SK_IVariable* fovy_;
      SK_IVariable* aspect_ratio_;

    private:
      static CommandProcessor* pCommProc;
    };

    extern bool               fullscreen;

    extern uint32_t           width;
    extern uint32_t           height;

    extern IDirect3DSurface9* pPostProcessSurface;
    extern IDirect3DDevice9*  pDevice;
    extern HWND               hWndDevice;

    extern volatile ULONG     dwRenderThreadID;
    extern bool               bink; // True if a bink video is playing

    extern HMODULE            d3dx9_43_dll;
    extern HMODULE            user32_dll;

    struct frame_state_s
    {
      void clear (void) { pixel_shaders.clear (); vertex_shaders.clear (); }
    
      std::unordered_set <uint32_t> pixel_shaders;
      std::unordered_set <uint32_t> vertex_shaders;
    } extern last_frame;

    struct render_target_tracking_s
    {
      void clear (void) { pixel_shaders.clear (); vertex_shaders.clear (); active = false; }

      IDirect3DBaseTexture9*        tracking_tex  = nullptr;

      std::unordered_set <uint32_t> pixel_shaders;
      std::unordered_set <uint32_t> vertex_shaders;

      bool                          active        = false;
    } extern tracked_rt;

    struct shader_tracking_s
    {
      void clear (void) { active = false; num_draws = 0; textures.clear (); }

      uint32_t                      crc32        =  0x00;
      bool                          cancel_draws = false;
      bool                          active       = false;
      int                           num_draws    =     0;
      std::unordered_set <uint32_t> textures;
    } extern tracked_vs, tracked_ps;

    struct shader_disasm_s {
      std::string header;
      std::string code;
      std::string footer;
    };
  }
}

struct game_state_t {
  BYTE*  base_addr    =  (BYTE *)0x2130309;
  bool   in_skit      = false;

  struct data_t {
    /*  0 */ BYTE  Title;
    /*  1 */ BYTE  OpeningMovie;
    /*  2 */ BYTE  Game;
    /*  3 */ BYTE  GamePause;
    /*  4 */ BYTE  Loading0;
    /*  5 */ BYTE  Loading1;
    /*  6 */ BYTE  Explanation;
    /*  7 */ BYTE  Menu;
    /*  8 */ BYTE  Unknown0;
    /*  9 */ BYTE  Unknown1;
    /* 10 */ BYTE  Unknown2;
    /* 11 */ BYTE  Battle;
    /* 12 */ BYTE  BattlePause;
    /* 13 */ BYTE  BattleSummary;
    /* 14 */ BYTE  Unknown3;
    /* 15 */ BYTE  Unknown4;
    /* 16 */ BYTE  Unknown5;
    /* 17 */ BYTE  Unknown6;
    /* 18 */ BYTE  Unknown7;
    /* 19 */ BYTE  Cutscene;
    /* 20 */ BYTE  Unknown8;
    /* 21 */ BYTE  Unknown9;
    /* 22 */ BYTE  Unknown10;
    /* 23 */ BYTE  Unknown11;
    /* 24 */ BYTE  Unknown12;
    /* 25 */ BYTE  Unknown13;
    /* 26 */ BYTE  Unknown14;
    /* 27 */ BYTE  Unknown15;
    /* 28 */ BYTE  Unknown16;
    /* 29 */ BYTE  Unknown17;
    /* 30 */ BYTE  GameMenu;
  };

#if 0
  BYTE*  Title        =  (BYTE *) base_addr;       // Title
  BYTE*  OpeningMovie =  (BYTE *)(base_addr + 1);  // Opening Movie

  BYTE*  Game         =  (BYTE *)(base_addr + 2);  // Game
  BYTE*  GamePause    =  (BYTE *)(base_addr + 3);  // Game Pause

  SHORT* Loading      = (SHORT *)(base_addr + 4);  // Why are there 2 states for this?

  BYTE*  Explanation  =  (BYTE *)(base_addr + 6);  // Explanation (+ Bink)?
  BYTE*  Menu         =  (BYTE *)(base_addr + 7);  // Menu

  BYTE*  Unknown0     =  (BYTE *)(base_addr + 8);  // Unknown
  BYTE*  Unknown1     =  (BYTE *)(base_addr + 9);  // Unknown - Appears to be battle related
  BYTE*  Unknown2     =  (BYTE *)(base_addr + 10); // Unknown

  BYTE*  Battle       =  (BYTE *)(base_addr + 11); // Battle
  BYTE*  BattlePause  =  (BYTE *)(base_addr + 12); // Battle Pause

  BYTE*  Unknown3     =  (BYTE *)(base_addr + 13); // Unknown
  BYTE*  Unknown4     =  (BYTE *)(base_addr + 14); // Unknown
  BYTE*  Unknown5     =  (BYTE *)(base_addr + 15); // Unknown
  BYTE*  Unknown6     =  (BYTE *)(base_addr + 16); // Unknown
  BYTE*  Unknown7     =  (BYTE *)(base_addr + 17); // Unknown
  BYTE*  Unknown8     =  (BYTE *)(base_addr + 18); // Unknown

  BYTE*  Cutscene     =  (BYTE *)(base_addr + 19); // Cutscene
#endif

  bool inBattle      (void) { return ((data_t *)base_addr)->Battle      & 0x1; }
  bool inCutscene    (void) { return ((data_t *)base_addr)->Cutscene    & 0x1; }
  bool inExplanation (void) { return ((data_t *)base_addr)->Explanation & 0x1; }
  bool inSkit        (void) { return in_skit;                                  }
  bool inMenu        (void) { return ((data_t *)base_addr)->Menu        & 0x1; }
  bool isLoading     (void) { return (((data_t *)base_addr)->Loading0 & 0x1) ||
                                     (((data_t *)base_addr)->Loading1 & 0x1); }

  bool hasFixedAspect (void) {
    return false;
    //if (((data_t *)base_addr)->OpeningMovie ||
        //((data_t *)base_addr)->Title        ||
        //((data_t *)base_addr)->Menu         ||
        //in_skit)
      //return true;
    //return false;
  }
  bool needsFixedMouseCoords(void) {
    return false;
    //return (((data_t *)base_addr)->GamePause    ||
            //((data_t *)base_addr)->Menu         ||
            //((data_t *)base_addr)->BattlePause  ||
            //((data_t *)base_addr)->Title);
  }
} static game_state;

typedef HRESULT (STDMETHODCALLTYPE *BeginScene_pfn)(
  IDirect3DDevice9 *This
);

typedef HRESULT (STDMETHODCALLTYPE *EndScene_pfn)(
  IDirect3DDevice9 *This
);

#include <d3d9.h>

typedef D3DPRESENT_PARAMETERS* (__stdcall *SK_SetPresentParamsD3D9_pfn)
(
  IDirect3DDevice9      *pDevice,
  D3DPRESENT_PARAMETERS *pParams
);

typedef HRESULT (STDMETHODCALLTYPE *DrawPrimitive_pfn)
(
  IDirect3DDevice9 *This,
  D3DPRIMITIVETYPE  PrimitiveType,
  UINT              StartVertex,
  UINT              PrimitiveCount
);

typedef HRESULT (STDMETHODCALLTYPE *DrawIndexedPrimitive_pfn)
(
  IDirect3DDevice9 *This,
  D3DPRIMITIVETYPE  Type,
  INT               BaseVertexIndex,
  UINT              MinVertexIndex,
  UINT              NumVertices,
  UINT              startIndex,
  UINT              primCount
);

typedef HRESULT (STDMETHODCALLTYPE *DrawPrimitiveUP_pfn)
(
  IDirect3DDevice9 *This,
  D3DPRIMITIVETYPE  PrimitiveType,
  UINT              PrimitiveCount,
  const void       *pVertexStreamZeroData,
  UINT              VertexStreamZeroStride
);

typedef HRESULT (STDMETHODCALLTYPE *DrawIndexedPrimitiveUP_pfn)
(
  IDirect3DDevice9 *This,
  D3DPRIMITIVETYPE  PrimitiveType,
  UINT              MinVertexIndex,
  UINT              NumVertices,
  UINT              PrimitiveCount,
  const void       *pIndexData,
  D3DFORMAT         IndexDataFormat,
  const void       *pVertexStreamZeroData,
  UINT              VertexStreamZeroStride
);



#endif /* __TZF__RENDER_H__ */