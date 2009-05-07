/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_listc.cpp
// Purpose:     XRC resource for wxListCtrl
// Author:      Brian Gavin
// Created:     2000/09/09
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_LISTCTRL

#include "wx/xrc/xh_listc.h"

#ifndef WX_PRECOMP
    #include "wx/textctrl.h"
#endif

#include "wx/listctrl.h"
#include "wx/imaglist.h"


IMPLEMENT_DYNAMIC_CLASS(wxListCtrlXmlHandler, wxXmlResourceHandler)

wxListCtrlXmlHandler::wxListCtrlXmlHandler()
: wxXmlResourceHandler()
{
    // wxListItem styles
    XRC_ADD_STYLE(wxLIST_FORMAT_LEFT);
    XRC_ADD_STYLE(wxLIST_FORMAT_RIGHT);
    XRC_ADD_STYLE(wxLIST_FORMAT_CENTRE);
    XRC_ADD_STYLE(wxLIST_MASK_STATE);
    XRC_ADD_STYLE(wxLIST_MASK_TEXT);
    XRC_ADD_STYLE(wxLIST_MASK_IMAGE);
    XRC_ADD_STYLE(wxLIST_MASK_DATA);
    XRC_ADD_STYLE(wxLIST_MASK_WIDTH);
    XRC_ADD_STYLE(wxLIST_MASK_FORMAT);
    XRC_ADD_STYLE(wxLIST_STATE_DONTCARE);
    XRC_ADD_STYLE(wxLIST_STATE_DROPHILITED);
    XRC_ADD_STYLE(wxLIST_STATE_FOCUSED);
    XRC_ADD_STYLE(wxLIST_STATE_SELECTED);
    XRC_ADD_STYLE(wxLIST_STATE_CUT);

    // wxListCtrl styles
    XRC_ADD_STYLE(wxLC_LIST);
    XRC_ADD_STYLE(wxLC_REPORT);
    XRC_ADD_STYLE(wxLC_ICON);
    XRC_ADD_STYLE(wxLC_SMALL_ICON);
    XRC_ADD_STYLE(wxLC_ALIGN_TOP);
    XRC_ADD_STYLE(wxLC_ALIGN_LEFT);
    XRC_ADD_STYLE(wxLC_AUTOARRANGE);
    XRC_ADD_STYLE(wxLC_USER_TEXT);
    XRC_ADD_STYLE(wxLC_EDIT_LABELS);
    XRC_ADD_STYLE(wxLC_NO_HEADER);
    XRC_ADD_STYLE(wxLC_SINGLE_SEL);
    XRC_ADD_STYLE(wxLC_SORT_ASCENDING);
    XRC_ADD_STYLE(wxLC_SORT_DESCENDING);
    XRC_ADD_STYLE(wxLC_VIRTUAL);
    XRC_ADD_STYLE(wxLC_HRULES);
    XRC_ADD_STYLE(wxLC_VRULES);
    XRC_ADD_STYLE(wxLC_NO_SORT_HEADER);
    AddWindowStyles();
}

wxObject *wxListCtrlXmlHandler::DoCreateResource()
{
    if (m_class == wxT("listitem"))
    {
        Handle_wxListItem();
        return m_parentAsWindow;
    }
    else
        return Handle_wxListCtrl();
}

bool wxListCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxListCtrl")) ||
            IsOfClass(node, wxT("listitem"));
}

long wxListCtrlXmlHandler::Handle_wxListItem()
{
    wxListCtrl * const list = wxDynamicCast(m_parentAsWindow, wxListCtrl);
    wxCHECK_MSG( list, -1, "must have wxListCtrl parent" );

    wxListItem item;

    if (HasParam(wxT("align")))
        item.SetAlign((wxListColumnFormat)GetStyle(wxT("align")));
    if (HasParam(wxT("bg")))
        item.SetBackgroundColour(GetColour(wxT("bg")));
    if (HasParam(wxT("col")))
        item.SetColumn((int)GetLong(wxT("col")));
    if (HasParam(wxT("data")))
        item.SetData(GetLong(wxT("data")));
    if (HasParam(wxT("font")))
        item.SetFont(GetFont());
    if (HasParam(wxT("mask")))
        item.SetMask(GetStyle(wxT("mask")));
    if (HasParam(wxT("state")))
        item.SetState(GetStyle(wxT("state")));
    if (HasParam(wxT("statemask")))
        item.SetStateMask(GetStyle(wxT("statemask")));
    if (HasParam(wxT("text")))
        item.SetText(GetText(wxT("text")));
    if (HasParam(wxT("textcolour")))
        item.SetTextColour(GetColour(wxT("textcolour")));
    if (HasParam(wxT("textcolor")))
        item.SetTextColour(GetColour(wxT("textcolor")));
    if (HasParam(wxT("width")))
        item.SetWidth((int)GetLong(wxT("width")));

    // the list control icon style, may be 0
    int image;
    if ( list->HasFlag(wxLC_ICON) )
        image = GetImageIndex(list, wxIMAGE_LIST_NORMAL);
    else if ( list->HasFlag(wxLC_SMALL_ICON) )
        image = GetImageIndex(list, wxIMAGE_LIST_SMALL);
    else
        image = wxNOT_FOUND;

    if ( image != wxNOT_FOUND )
        item.SetImage(image);

    // append the list item to the control
    item.SetId(list->GetItemCount());

    return list->InsertItem(item);
}

wxObject* wxListCtrlXmlHandler::Handle_wxListCtrl()
{
    XRC_MAKE_INSTANCE(list, wxListCtrl)

    list->Create(m_parentAsWindow,
                 GetID(),
                 GetPosition(), GetSize(),
                 GetStyle(),
                 wxDefaultValidator,
                 GetName());

    // we can optionally have normal and/or small image lists
    wxImageList *imagelist;
    imagelist = GetImageList(wxT("imagelist"));
    if ( imagelist )
        list->AssignImageList(imagelist, wxIMAGE_LIST_NORMAL);
    imagelist = GetImageList(wxT("imagelist-small"));
    if ( imagelist )
        list->AssignImageList(imagelist, wxIMAGE_LIST_SMALL);

    CreateChildrenPrivately(list);
    SetupWindow(list);

    return list;
}

long wxListCtrlXmlHandler::GetImageIndex(wxListCtrl *listctrl, int which)
{
    // use different tag names depending on whether we need a normal or small
    // image
    wxString
        bmpParam("bitmap"),
        imgParam("image");
    switch ( which )
    {
        case wxIMAGE_LIST_SMALL:
            bmpParam += "-small";
            imgParam += "-small";
            break;

        case wxIMAGE_LIST_NORMAL:
            // nothing to do
            break;

        default:
            wxFAIL_MSG( "unsupported image list kind" );
            return wxNOT_FOUND;
    }

    // look for either bitmap or image tags
    int imgIndex = wxNOT_FOUND;
    if ( HasParam(bmpParam) )
    {
        // we implicitly construct an image list containing the specified
        // bitmaps
        wxBitmap bmp = GetBitmap(bmpParam, wxART_OTHER);

        // create the image list on demand for the first bitmap
        wxImageList *imgList = listctrl->GetImageList(which);
        if ( !imgList )
        {
            imgList = new wxImageList( bmp.GetWidth(), bmp.GetHeight() );
            listctrl->AssignImageList( imgList, which );
        }

        imgIndex = imgList->Add(bmp);
    }

    if ( HasParam(imgParam) )
    {
        if ( imgIndex != wxNOT_FOUND )
        {
            // TODO: we should really check that only bitmap or only image tags
            //       are used across all items of the control, not just in this
            //       one
            ReportError(wxString::Format(
                "listitem %s attribute ignored because %s is also specified",
                bmpParam, imgParam));
        }

        // just use the specified index directly
        imgIndex = GetLong(imgParam);
    }

    return imgIndex;
}

#endif // wxUSE_XRC && wxUSE_LISTCTRL
