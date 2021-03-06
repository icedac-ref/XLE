﻿// Copyright 2015 XLGAMES Inc.
//
// Distributed under the MIT License (See
// accompanying file "LICENSE" or the website
// http://www.opensource.org/licenses/mit-license.php)

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace LevelEditorXLE.Manipulators
{
    public partial class XLENativeManipControls : UserControl
    {
        public XLENativeManipControls()
        {
            InitializeComponent();
            Layout += XLETerrainManipControls_Layout;
        }

        private void XLETerrainManipControls_Layout(object sender, LayoutEventArgs e)
        {
                // resize children based on the orientation of this window
            var size = ClientSize;
            if (size.Width > size.Height)
            {
                var selectionWidth = 149;
                manipulatorSelection.Size = new System.Drawing.Size(selectionWidth, size.Height - 6);
                manipulatorProperties.Location = new System.Drawing.Point(selectionWidth + 9, 3);
                manipulatorProperties.Size = new System.Drawing.Size(size.Width - selectionWidth - 12, size.Height - 6);
            }
            else
            {
                var h = manipulatorSelection.PreferredHeight;
                manipulatorSelection.Size = new System.Drawing.Size(size.Width - 6, h);
                manipulatorProperties.Location = new System.Drawing.Point(3, h + 9);
                manipulatorProperties.Size = new System.Drawing.Size(size.Width - 6, size.Height - h - 12);
            }
        }

        private void manipulatorSelection_SelectedIndexChanged(object sender, EventArgs e)
        {
            var newManipulator = manipulatorSelection.SelectedItem as string;
            if (newManipulator != null && _context != null)
            {
                _context.ActiveManipulator = newManipulator;
            }
        }

        public void SetActiveContext(GUILayer.ActiveManipulatorContext context)
        {
            if (_context != null)
            {
                _context.OnActiveManipulatorChange -= OnActiveManipulatorChange;
                _context.OnManipulatorSetChange -= OnManipulatorSetChange;
            }
            manipulatorProperties.Bind(null);

            _context = context;
            if (_context != null)
            {
                _context.OnActiveManipulatorChange += OnActiveManipulatorChange;
                _context.OnManipulatorSetChange += OnManipulatorSetChange;
            }

            OnManipulatorSetChange(null, null);
        }

        public void SetFocusProperties(object properties)
        {
            manipulatorProperties.Bind(properties as Sce.Atf.Applications.IPropertyEditingContext);
        }

        private GUILayer.ActiveManipulatorContext _context = null;

        private void OnActiveManipulatorChange(object sender, EventArgs args)
        {
            if (_context != null && _context.ManipulatorSet != null && _context.ActiveManipulator != null)
            {
                SetFocusProperties(
                    new XLEBridgeUtils.PropertyBridge(_context.ManipulatorSet.GetProperties(_context.ActiveManipulator)));
            }
            else
            {
                SetFocusProperties(null);
            }
        }

        private void OnManipulatorSetChange(object sender, EventArgs args)
        {
            if (_context != null && _context.ManipulatorSet != null)
            {
                var names = _context.ManipulatorSet.GetManipulatorNames();
                manipulatorSelection.DataSource = names;
            }
            else
            {
                manipulatorSelection.DataSource = null;
            }

            OnActiveManipulatorChange(null, null);
        }
    }
}
