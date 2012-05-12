/*
 Copyright (C) 2010-2012 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DragTool.h"
#include "Controller/DragPlane.h"

namespace TrenchBroom {
    namespace Controller {
        void DragTool::updateDragPlane(ToolEvent& event) {
            if (m_dragPlane != NULL) {
                delete m_dragPlane;
                m_dragPlane = NULL;
            }
            
            if (altPlaneModifierPressed(event))
                m_dragPlane = new DragPlane(event.ray.direction);
            else
                m_dragPlane = new DragPlane();
        }

        bool DragTool::doBeginLeftDrag(ToolEvent& event, Vec3f& lastPoint) { return false; }
        bool DragTool::doLeftDrag(ToolEvent& event, Vec3f& delta, Vec3f& lastPoint) { return false; }
        void DragTool::doEndLeftDrag(ToolEvent& event) {}
        
        bool DragTool::doBeginRightDrag(ToolEvent& event, Vec3f& lastPoint) { return false; }
        bool DragTool::doRightDrag(ToolEvent& event, Vec3f& delta, Vec3f& lastPoint) { return false; }
        void DragTool::doEndRightDrag(ToolEvent& event) {}

        bool DragTool::altPlaneModifierPressed(ToolEvent& event) {
            return event.modifierKeys == TB_MK_ALT;
        }

        DragTool::DragTool(Editor& editor) : Tool(editor), m_dragPlane(NULL), m_dragPlanePosition(Null3f), m_lastPoint(Null3f), m_drag(false) {};
        
        DragTool::~DragTool() {
            if (m_dragPlane != NULL)
                delete m_dragPlane;
        }
        
        bool DragTool::beginLeftDrag(ToolEvent& event) {
            updateDragPlane(event);
            m_drag = doBeginLeftDrag(event, m_lastPoint);
            m_dragPlanePosition = m_lastPoint;
            return m_drag;
        }
        
        void DragTool::leftDrag(ToolEvent& event) {
            if (!m_drag)
                return;
            
            float dist = m_dragPlane->intersect(event.ray, m_dragPlanePosition);
            if (isnan(dist))
                return;
            
            Vec3f point = event.ray.pointAtDistance(dist);
            Vec3f delta = point - m_lastPoint;
            if (!doLeftDrag(event, delta, m_lastPoint))
                endLeftDrag(event);
        }

        void DragTool::endLeftDrag(ToolEvent& event) {
            if (!m_drag)
                return;
            
            doEndLeftDrag(event);
            
            if (m_dragPlane != NULL) {
                delete m_dragPlane;
                m_dragPlane = NULL;
            }
            
            m_drag = false;
        }
    }
}