/*
* @file    ds68_streamselect.cpp
* @brief   sc68 for directshow - track info
* @author  http://sourceforge.net/users/benjihan
*
* Copyright (c) 1998-2014 Benjamin Gerard
*
* This program is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.
*
* If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ds68_headers.h"

#ifdef WITH_STREAMSELECT

HRESULT Sc68Splitter::Count(DWORD *pcStreams)
{
  HRESULT hr;
  if (!pcStreams)
    hr = E_POINTER;
  else if (m_info.tracks <= 0)
    hr = VFW_E_NOT_CONNECTED;
  else {
    hr = S_OK;
    *pcStreams = m_info.tracks + (m_info.tracks > 1);
  }
  DBG("%s() => %d\n", __FUNCTION__, !hr ? *pcStreams : -1);
  return hr;
}

HRESULT Sc68Splitter::Enable(long lIndex,DWORD dwFlags)
{
  HRESULT hr = E_NOTIMPL;

  switch (dwFlags) {
  case 0:
    // Disable all streams in the group containing this stream.
    break;
  case AMSTREAMSELECTENABLE_ENABLE:
    // Enable only this stream within the given group and disable all others.
    break;
  case AMSTREAMSELECTENABLE_ENABLEALL:
    // Enable all streams in the group containing this stream.
    break;
  default:
    break;
  }

  DBG("%s() idx:%d flags:%d -> %s(%d)\n", __FUNCTION__,
    lIndex, dwFlags, !hr?"OK":"FAIL",hr);
  return hr;
}

HRESULT Sc68Splitter::Info(long lIndex,
    AM_MEDIA_TYPE **ppmt, DWORD *pdwFlags, LCID *plcid, DWORD *pdwGroup,
    WCHAR **ppszName, IUnknown **ppObject, IUnknown **ppUnk)
{
  HRESULT hr = E_FAIL;

  if (m_info.tracks <= 0)
    hr = E_FAIL;
  else if (lIndex < 0 || lIndex > m_info.tracks)
    hr = S_FALSE;
  else {
    if (ppmt) {
      *ppmt = NULL; // TODO
    }
    if (lIndex == 0) {
    }
    else {
    }
  }
  DBG("%s() => %s\n", __FUNCTION__, !hr ? "OK":"FAIL");
  return hr;
}
#endif
