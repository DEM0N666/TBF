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
#ifndef __TBF__FRAMERATE_H__
#define __TBF__FRAMERATE_H__

#include <stdint.h>
#include <Windows.h>

#include "command.h"

namespace tbf
{
  namespace FrameRateFix
  {
    void Init     (void);
    void Shutdown (void);


    // Called whenever the engine finishes a frame
    void RenderTick (void);


    // Call when re-engaging the framerate limiter to make it match
    //   user prefrences.
    void BlipFramerate    (void);

    // Disable Special K's framerate limit, generally only useful
    //   when demonstrating what is wrong with Namco's.
    void DisengageLimiter (void);

    float GetTargetFrametime (void);


#if 0
    //
    // At key points during the game, we need to disable the code that
    //  cuts timing in half. These places will be wrapped by calls to
    //    these.
    //
    void Begin30FPSEvent (void);
    void End30FPSEvent   (void);

    void SetFPS          (int fps);
#endif

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
      SK_IVariable* tick_scale_;

    private:
      static CommandProcessor* pCommProc;
    };

    // True if the executable has been modified (at run-time) to allow 60 FPS
    extern bool             variable_speed_installed;

    // Allow Namco's limiter to run one frame to propogate user setting changes
    extern bool             need_reset;

    // This is actually setup in the SK DLL that loads this one
    extern uint32_t         target_fps;

    // Cache the game's tick scale for timing -- this can be changed without
    //   our knowledge, so this is more or less a hint rather than a rule
    extern int32_t          tick_scale;

    // Prevent mult-threaded shenanigans
    extern CRITICAL_SECTION alter_speed_cs;

    // Hold references to these DLLs
    extern HMODULE          bink_dll;
    extern HMODULE          kernel32_dll;
  }
}

#endif /* __TBF__FRAMERATE_H__ */