/*
 * @file    ds68_trackinfo.cpp
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

#ifdef WITH_TRACKINFO

STDMETHODIMP_(UINT)
Sc68Splitter::GetTrackCount()
{
  UINT res = m_allin1 ? m_info.tracks > 0 : m_info.tracks;
  DBG("%s() => %u\n", res);
  return res;
}

STDMETHODIMP_(BOOL)
Sc68Splitter::GetTrackInfo(UINT aTrackIdx, struct TrackElement * pStructureToFill)
{
  if (m_allin1 || aTrackIdx >= (UINT)m_info.tracks || !pStructureToFill)
    return false;
  pStructureToFill->FlagDefault = aTrackIdx == 0;
  pStructureToFill->FlagForced  = false;
  pStructureToFill->FlagLacing  = false;
  pStructureToFill->MaxCache    = 0;
  pStructureToFill->MinCache    = 0;
  pStructureToFill->Size        = sizeof(*pStructureToFill);
  pStructureToFill->Type        = TypeAudio;
  return true;
}

STDMETHODIMP_(BOOL)
Sc68Splitter::GetTrackExtendedInfo(UINT aTrackIdx, void* pStructureToFill)
{
  struct TrackExtendedInfoAudio * p
    = (struct TrackExtendedInfoAudio *)pStructureToFill;

  if (m_allin1 || aTrackIdx >= (UINT)m_info.tracks || !pStructureToFill)
    return false;
  p->BitDepth = 16;
  p->Channels = 2;
  p->OutputSamplingFrequency = (float)m_spr;
  p->SamplingFreq = (float)m_spr;
  p->Size = sizeof(*p);
  return true;
}


STDMETHODIMP_(BSTR)
Sc68Splitter::GetTrackCodecID(UINT aTrackIdx)
{
  BSTR str = nullptr;
  if (m_info.genre && *m_info.genre)
    BSTRset(&str, m_info.genre);
  return nullptr;
}

STDMETHODIMP_(BSTR)
Sc68Splitter::GetTrackName(UINT aTrackIdx)
{
  BSTR str = nullptr;
  if (!m_allin1 && aTrackIdx < (UINT)m_info.tracks) {
    if (m_info.tracks == 1 && m_info.album)
      BSTRset(&str, m_info.album);
    else {
      sc68_minfo_t info;
      if (!sc68_music_info(m_sc68,&info,aTrackIdx,0)) {
        char tmp[256];
        if (info.title)
          _snprintf(tmp,sizeof(tmp),"%02d. %s",info.trk.track, info.title);
        else if (info.album)
          _snprintf(tmp,sizeof(tmp),"%02d. %s",info.trk.track, info.album);
        else
          _snprintf(tmp,sizeof(tmp),"track #%02d",info.trk.track);
        tmp[sizeof(tmp)-1] = 0;
        BSTRset(&str,tmp);
      }
    }
  }
  return str;
}

STDMETHODIMP_(BSTR)
Sc68Splitter::GetTrackCodecName(UINT aTrackIdx)
{
  return GetTrackCodecID(aTrackIdx);
}

STDMETHODIMP_(BSTR)
Sc68Splitter::GetTrackCodecInfoURL(UINT aTrackIdx)
{
  return nullptr;
}

STDMETHODIMP_(BSTR)
Sc68Splitter::GetTrackCodecDownloadURL(UINT aTrackIdx)
{
  return nullptr;
}

#endif
