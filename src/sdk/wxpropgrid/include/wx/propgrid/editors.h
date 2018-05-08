/////////////////////////////////////////////////////////////////////////////
// Name:        editors.h
// Purpose:     wxPropertyGrid editors
// Author:      Jaakko Salli
// Modified by:
// Created:     Apr-14-2007
// RCS-ID:      $Id:
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PROPGRID_EDITORS_H_
#define _WX_PROPGRID_EDITORS_H_

// -----------------------------------------------------------------------
// wxPGWindowList contains list of editor windows returned by CreateControls.

class wxPGWindowList
{
public:
    wxPGWindowList()
    {
        m_primary = m_secondary = NULL;
    }

    void SetSecondary( wxWindow* secondary ) { m_secondary = secondary; }

    wxWindow*   m_primary;
    wxWindow*   m_secondary;

#ifndef SWIG
    wxPGWindowList( wxWindow* a )
    {
        m_primary = a;
        m_secondary = NULL;
    };
    wxPGWindowList( wxWindow* a, wxWindow* b )
    {
        m_primary = a;
        m_secondary = b;
    };
#endif
};

// -----------------------------------------------------------------------

/** @class wxPGEditor
    @ingroup classes
    @brief Base for property editor classes.
    @remarks
    - Event handling:
      wxPGEditor::CreateControls should Connect all necessary events to the
      wxPropertyGrid::OnCustomEditorEvent. For Example:
        @code
            // Relays wxEVT_COMMAND_TEXT_UPDATED events of primary editor
            // control to the OnEvent.
            // NOTE: This event in particular is actually automatically conveyed, but
            //   it is just used as an example.
            propgrid->Connect( wxPG_SUBID1, wxEVT_COMMAND_TEXT_UPDATED,
                              (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)
                              &wxPropertyGrid::OnCustomEditorEvent );
        @endcode
      OnCustomEditorEvent will then forward events, first to wxPGEditor::OnEvent
      and then to wxPGProperty::OnEvent.
    - You need to call macro wxPGRegisterEditorClass(EditorName) before using a custom editor class.
    - See propgrid.cpp for how builtin editors works (starting from wxPGTextCtrlEditor).
*/
class WXDLLIMPEXP_PG wxPGEditor : public wxObject
{
#ifndef SWIG
    DECLARE_ABSTRACT_CLASS(wxPGEditor)
#endif
public:

    /** Constructor. */
    wxPGEditor()
        : wxObject()
    {
    #if defined(__WXPYTHON__)
        m_scriptObject = NULL;
    #endif
    }

    /** Destructor. */
    ~wxPGEditor() override;

    /** Returns pointer to the name of the editor. For example, wxPG_EDITOR(TextCtrl)
        has name "TextCtrl". This method is autogenerated for custom editors.
    */
    virtual wxPG_CONST_WXCHAR_PTR GetName() const = 0;

    /** Instantiates editor controls.
        @remarks
        - Primary control shall use id wxPG_SUBID1, and secondary (button) control
          shall use wxPG_SUBID2.
        @param propgrid
        wxPropertyGrid to which the property belongs (use as parent for control).
        @param property
        Property for which this method is called.
        @param pos
        Position, inside wxPropertyGrid, to create control(s) to.
        @param size
        Initial size for control(s).
    */
    virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property,
        const wxPoint& pos, const wxSize& size ) const = 0;
    #define wxPG_DECLARE_CREATECONTROLS \
        virtual wxPGWindowList CreateControls( wxPropertyGrid* propgrid, wxPGProperty* property, \
        const wxPoint& pos, const wxSize& sz ) const;

    /** Loads value from property to the control. */
    virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const = 0;

    /** Used to get the renderer to draw the value with when the control is hidden.

        Default implementation returns g_wxPGDefaultRenderer.
    */
    //virtual wxPGCellRenderer* GetCellRenderer() const;

    /** Draws value for given property.
    */
    virtual void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const;

    /** Handles events. Returns true if value in control was modified
        (see wxPGProperty::OnEvent for more information).
    */
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property,
        wxWindow* wnd_primary, wxEvent& event ) const = 0;

#ifndef DOXYGEN
private:
#else
public:
#endif
    /** Returns value from control, via parameter 'variant'.
        Usually ends up calling property's StringToValue or IntToValue.
        Returns true if value was different.
    */
    virtual bool GetValueFromControl( wxVariant& variant, wxPGProperty* property, wxWindow* ctrl ) const WX_PG_NOT_PURE_IN_WXPYTHON;
public:

#ifdef __WXPYTHON__
    virtual wxPGVariantAndBool PyGetValueFromControl( wxPGProperty* property, wxWindow* ctrl ) const;
#endif

    bool ActualGetValueFromControl( wxVariant& variant, wxPGProperty* property, wxWindow* ctrl ) const
    {
    #ifdef __WXPYTHON__
        if ( m_scriptObject )
        {
            wxPGVariantAndBool vab = PyGetValueFromControl(property, ctrl);
            if ( vab.m_valueValid )
                variant = vab.m_value;
            return vab.m_result;
        }
    #endif
        return GetValueFromControl(variant, property, ctrl);
    }

    /** Sets value in control to unspecified. */
    virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const = 0;

    /** Sets control's value specifically from string. */
    virtual void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const;

    /** Sets control's value specifically from int (applies to choice etc.). */
    virtual void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const;

    /** Inserts item to existing control. Index -1 means appending.
        Default implementation does nothing. Returns index of item added.
    */
    virtual int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const;

    /** Deletes item from existing control.
        Default implementation does nothing.
    */
    virtual void DeleteItem( wxWindow* ctrl, int index ) const;

    /** Extra processing when control gains focus. For example, wxTextCtrl 
        based controls should select all text.
    */
    virtual void OnFocus( wxPGProperty* property, wxWindow* wnd ) const;

    /** Returns true if control itself can contain the custom image. Default is
        to return false.
    */
    virtual bool CanContainCustomImage() const;

#if defined(__WXPYTHON__) && !defined(SWIG)
    // This is the python object that contains and owns the C++ representation.
    PyObject*                   m_scriptObject;
#endif

protected:
};


//
// Note that we don't use this macro in this file because
// otherwise doxygen gets confused.
//
#define WX_PG_DECLARE_EDITOR_CLASS(CLASSNAME) \
    DECLARE_DYNAMIC_CLASS(CLASSNAME) \
public: \
    virtual wxPG_CONST_WXCHAR_PTR GetName() const; \
private:


#define WX_PG_IMPLEMENT_EDITOR_CLASS(EDITOR,CLASSNAME,BASECLASS) \
IMPLEMENT_DYNAMIC_CLASS(CLASSNAME, BASECLASS) \
wxPG_CONST_WXCHAR_PTR CLASSNAME::GetName() const \
{ \
    return wxT(#EDITOR); \
} \
wxPGEditor* wxPGEditor_##EDITOR = NULL; \
wxPGEditor* wxPGConstruct##EDITOR##EditorClass() \
{ \
    wxASSERT( !wxPGEditor_##EDITOR ); \
    return new CLASSNAME(); \
}


#define WX_PG_IMPLEMENT_EDITOR_CLASS_STD_METHODS() \
wxPG_DECLARE_CREATECONTROLS \
virtual void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const; \
virtual bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property, \
    wxWindow* primary, wxEvent& event ) const; \
virtual bool GetValueFromControl( wxVariant& variant, wxPGProperty* property, wxWindow* ctrl ) const; \
virtual void SetValueToUnspecified( wxPGProperty* property, wxWindow* ctrl ) const;


//
// Following are the built-in editor classes.
//

class WXDLLIMPEXP_PG wxPGTextCtrlEditor : public wxPGEditor
{
#ifndef SWIG
    WX_PG_DECLARE_EDITOR_CLASS(wxPGTextCtrlEditor)
#endif
public:
    wxPGTextCtrlEditor() {}
    ~wxPGTextCtrlEditor() override;

    WX_PG_IMPLEMENT_EDITOR_CLASS_STD_METHODS()

    //virtual wxPGCellRenderer* GetCellRenderer() const;
    void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const override;
    void OnFocus( wxPGProperty* property, wxWindow* wnd ) const override;

    // Provided so that, for example, ComboBox editor can use the same code
    // (multiple inheritance would get way too messy).
    static bool OnTextCtrlEvent( wxPropertyGrid* propgrid,
                                 wxPGProperty* property,
                                 wxWindow* ctrl,
                                 wxEvent& event );

    static bool GetTextCtrlValueFromControl( wxVariant& variant, wxPGProperty* property, wxWindow* ctrl );

};


class WXDLLIMPEXP_PG wxPGChoiceEditor : public wxPGEditor
{
#ifndef SWIG
    WX_PG_DECLARE_EDITOR_CLASS(wxPGChoiceEditor)
#endif
public:
    wxPGChoiceEditor() {}
    ~wxPGChoiceEditor() override;

    WX_PG_IMPLEMENT_EDITOR_CLASS_STD_METHODS()

    void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const override;
    void SetControlStringValue( wxPGProperty* property, wxWindow* ctrl, const wxString& txt ) const override;

    int InsertItem( wxWindow* ctrl, const wxString& label, int index ) const override;
    void DeleteItem( wxWindow* ctrl, int index ) const override;
    bool CanContainCustomImage() const override;

    // CreateControls calls this with CB_READONLY in extraStyle
    wxWindow* CreateControlsBase( wxPropertyGrid* propgrid,
                                  wxPGProperty* property,
                                  const wxPoint& pos,
                                  const wxSize& sz,
                                  long extraStyle ) const;

};


class WXDLLIMPEXP_PG wxPGComboBoxEditor : public wxPGChoiceEditor
{
#ifndef SWIG
    WX_PG_DECLARE_EDITOR_CLASS(wxPGComboBoxEditor)
#endif
public:
    wxPGComboBoxEditor() {}
    ~wxPGComboBoxEditor() override;

    wxPG_DECLARE_CREATECONTROLS  // Macro is used for conviency due to different signature with wxPython

    void UpdateControl( wxPGProperty* property, wxWindow* ctrl ) const override;

    bool OnEvent( wxPropertyGrid* propgrid, wxPGProperty* property,
        wxWindow* ctrl, wxEvent& event ) const override;

    bool GetValueFromControl( wxVariant& variant, wxPGProperty* property, wxWindow* ctrl ) const override;

    void OnFocus( wxPGProperty* property, wxWindow* wnd ) const override;

};


class WXDLLIMPEXP_PG wxPGChoiceAndButtonEditor : public wxPGChoiceEditor
{
#ifndef SWIG
    WX_PG_DECLARE_EDITOR_CLASS(wxPGChoiceAndButtonEditor)
#endif
public:
    wxPGChoiceAndButtonEditor() {}
    ~wxPGChoiceAndButtonEditor() override;
    wxPG_DECLARE_CREATECONTROLS  // Macro is used for conviency due to different signature with wxPython
};


class WXDLLIMPEXP_PG wxPGTextCtrlAndButtonEditor : public wxPGTextCtrlEditor
{
#ifndef SWIG
    WX_PG_DECLARE_EDITOR_CLASS(wxPGTextCtrlAndButtonEditor)
#endif
public:
    wxPGTextCtrlAndButtonEditor() {}
    ~wxPGTextCtrlAndButtonEditor() override;
    wxPG_DECLARE_CREATECONTROLS
};


#if wxPG_INCLUDE_CHECKBOX || defined(DOXYGEN)

//
// Use custom check box code instead of native control
// for cleaner (ie. more integrated) look.
//
class WXDLLIMPEXP_PG wxPGCheckBoxEditor : public wxPGEditor
{
#ifndef SWIG
    WX_PG_DECLARE_EDITOR_CLASS(wxPGCheckBoxEditor)
#endif
public:
    wxPGCheckBoxEditor() {}
    ~wxPGCheckBoxEditor() override;

    WX_PG_IMPLEMENT_EDITOR_CLASS_STD_METHODS()

    void DrawValue( wxDC& dc, const wxRect& rect, wxPGProperty* property, const wxString& text ) const override;
    //virtual wxPGCellRenderer* GetCellRenderer() const;

    void SetControlIntValue( wxPGProperty* property, wxWindow* ctrl, int value ) const override;
};

#endif


// -----------------------------------------------------------------------
// Editor class registeration macros

#define wxPGRegisterEditorClass(EDITOR) \
    if ( wxPGEditor_##EDITOR == (wxPGEditor*) NULL ) \
    { \
        wxPGEditor_##EDITOR = wxPropertyGrid::RegisterEditorClass( wxPGConstruct##EDITOR##EditorClass(), wxT(#EDITOR) ); \
    }

// Use this in RegisterDefaultEditors.
#define wxPGRegisterDefaultEditorClass(EDITOR) \
if ( wxPGEditor_##EDITOR == (wxPGEditor*) NULL ) \
    { \
        wxPGEditor_##EDITOR = wxPropertyGrid::RegisterEditorClass( wxPGConstruct##EDITOR##EditorClass(), wxT(#EDITOR), true ); \
    }

#define wxPG_INIT_REQUIRED_EDITOR(T) \
    wxPGRegisterEditorClass(T)


// -----------------------------------------------------------------------

/** @class wxPGEditorDialogAdapter
	@ingroup classes
    @brief
    Derive a class from this to adapt an existing editor dialog or function to
    be used when editor button of a property is pushed.

    You only need to derive class and implement DoShowDialog() to create and
    show the dialog, and finally submit the value returned by the dialog
    via SetValue().
*/
class WXDLLIMPEXP_PG wxPGEditorDialogAdapter : public wxObject
{
#ifndef SWIG
    DECLARE_ABSTRACT_CLASS(wxPGEditorDialogAdapter)
#endif
public:
    wxPGEditorDialogAdapter()
        : wxObject()
    {
#if defined(__WXPYTHON__)
        m_scriptObject = NULL;
#endif
    }

    ~wxPGEditorDialogAdapter() override { }

    bool ShowDialog( wxPropertyGrid* propGrid, wxPGProperty* property );

    virtual bool DoShowDialog( wxPropertyGrid* propGrid, wxPGProperty* property ) = 0;

    void SetValue( wxVariant value )
    {
        m_value = value;
    }

    /** This method is typically only used if deriving class from existing adapter
        with value conversion purposes.
    */
    wxVariant& GetValue()
    {
        return m_value;
    }

#if defined(__WXPYTHON__) && !defined(SWIG)
    // This is the python object that contains and owns the C++ representation.
    PyObject*                   m_scriptObject;
#endif
protected:

private:
    wxVariant           m_value;
};

// -----------------------------------------------------------------------


/** @class wxPGMultiButton
	@ingroup classes
    @brief
    This class can be used to have multiple buttons in a property editor.
    You will need to create a new property editor class, override CreateControls,
    and have it return wxPGMultiButton instance in wxPGWindowList::SetSecondary().
    For instance, here we add three buttons to a textctrl editor:

    @code

    #include <wx/propgrid/editors.h>

    class wxMultiButtonTextCtrlEditor : public wxPGTextCtrlEditor
    {
        WX_PG_DECLARE_EDITOR_CLASS(wxMultiButtonTextCtrlEditor)
    public:
        wxMultiButtonTextCtrlEditor() {}
        virtual ~wxMultiButtonTextCtrlEditor() {}

        wxPG_DECLARE_CREATECONTROLS
        virtual bool OnEvent( wxPropertyGrid* propGrid,
                              wxPGProperty* property,
                              wxWindow* ctrl,
                              wxEvent& event ) const;

    };

    WX_PG_IMPLEMENT_EDITOR_CLASS(MultiButtonTextCtrlEditor, wxMultiButtonTextCtrlEditor,
                                 wxPGTextCtrlEditor)

    wxPGWindowList wxMultiButtonTextCtrlEditor::CreateControls( wxPropertyGrid* propGrid,
                                                                wxPGProperty* property,
                                                                const wxPoint& pos,
                                                                const wxSize& sz ) const
    {
        // Create and populate buttons-subwindow
        wxPGMultiButton* buttons = new wxPGMultiButton( propGrid, sz );

        // Add two regular buttons
        buttons->Add( wxT("...") );
        buttons->Add( wxT("A") );
        // Add a bitmap button
        buttons->Add( wxArtProvider::GetBitmap(wxART_FOLDER) );

        // Create the 'primary' editor control (textctrl in this case)
        wxPGWindowList wndList = wxPGTextCtrlEditor::CreateControls
                                 ( propGrid, property, pos, buttons->GetPrimarySize() );

        // Finally, move buttons-subwindow to correct position and make sure
        // returned wxPGWindowList contains our custom button list.
        buttons->FinalizePosition(pos);

        wndList.SetSecondary( buttons );
        return wndList;
    }

    bool wxMultiButtonTextCtrlEditor::OnEvent( wxPropertyGrid* propGrid,
                                               wxPGProperty* property,
                                               wxWindow* ctrl,
                                               wxEvent& event ) const
    {
        if ( event.GetEventType() == wxEVT_COMMAND_BUTTON_CLICKED )
        {
            wxPGMultiButton* buttons = (wxPGMultiButton*) propGrid->GetEditorControlSecondary();

            if ( event.GetId() == buttons->GetButtonId(0) )
            {
                // Do something when first button is pressed,
                // Return true if value in editor changed by this
                // action.
                ...
            }
            if ( event.GetId() == buttons->GetButtonId(1) )
            {
                // Do something when second button is pressed
                ...
            }
            if ( event.GetId() == buttons->GetButtonId(2) )
            {
                // Do something when third button is pressed
                ...
            }
        }
        return wxPGTextCtrlEditor::OnEvent(propGrid, property, ctrl, event);
    }

    @endcode

    Further to use this editor, code like this can be used:

    @code

        // Register editor class - needs only to be called once
        wxPGRegisterEditorClass( MultiButtonTextCtrlEditor );

        // Insert the property that will have multiple buttons
        propGrid->Append( new wxLongStringProperty(wxT("MultipleButtons"), wxPG_LABEL) );

        // Change property to use editor created in the previous code segment
        propGrid->SetPropertyEditor( wxT("MultipleButtons"), wxPG_EDITOR(MultiButtonTextCtrlEditor) );

    @endcode
*/
class WXDLLIMPEXP_PG wxPGMultiButton : public wxWindow
{
public:

    wxPGMultiButton( wxPropertyGrid* pg, const wxSize& sz );

    ~wxPGMultiButton() override { }

    wxWindow* GetButton( unsigned int i ) { return (wxWindow*) m_buttons[i]; }
    const wxWindow* GetButton( unsigned int i ) const { return (const wxWindow*) m_buttons[i]; }

    /** Utility function to be used in event handlers.
    */
    int GetButtonId( unsigned int i ) const { return GetButton(i)->GetId(); }

    /** Returns number of buttons.
    */
    int GetCount() const { return (int) m_buttons.size(); }

    void Add( const wxString& label, int id = -2 );
#if wxUSE_BMPBUTTON
    void Add( const wxBitmap& bitmap, int id = -2 );
#endif

    wxSize GetPrimarySize() const
    {
        return wxSize(m_fullEditorSize.x - m_buttonsWidth, m_fullEditorSize.y);
    }

    void FinalizePosition( const wxPoint& pos )
    {
        Move( pos.x + m_fullEditorSize.x - m_buttonsWidth, pos.y );
    }

#ifndef DOXYGEN
protected:

    int GenId( int id ) const;

    wxArrayPtrVoid  m_buttons;
    wxSize          m_fullEditorSize;
    int             m_buttonsWidth;
#endif // !DOXYGEN
};


// -----------------------------------------------------------------------
// Utilities

WXDLLIMPEXP_PG bool wxPG_TextCtrl_SetMargins(wxWindow* tc,
                                             const wxPoint& margins);

// -----------------------------------------------------------------------

#endif // _WX_PROPGRID_EDITORS_H_
