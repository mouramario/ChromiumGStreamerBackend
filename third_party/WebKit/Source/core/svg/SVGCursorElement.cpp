/*
 * Copyright (C) 2004, 2005, 2006, 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007 Rob Buis <buis@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "core/svg/SVGCursorElement.h"

#include "core/SVGNames.h"
#include "core/dom/StyleChangeReason.h"
#include "core/frame/UseCounter.h"

namespace blink {

inline SVGCursorElement::SVGCursorElement(Document& document)
    : SVGElement(SVGNames::cursorTag, document),
      SVGTests(this),
      SVGURIReference(this),
      m_x(SVGAnimatedLength::create(this,
                                    SVGNames::xAttr,
                                    SVGLength::create(SVGLengthMode::Width))),
      m_y(SVGAnimatedLength::create(this,
                                    SVGNames::yAttr,
                                    SVGLength::create(SVGLengthMode::Height))) {
  addToPropertyMap(m_x);
  addToPropertyMap(m_y);

  UseCounter::count(document, UseCounter::SVGCursorElement);
}

DEFINE_NODE_FACTORY(SVGCursorElement)

SVGCursorElement::~SVGCursorElement() {}

void SVGCursorElement::addClient(SVGElement* element) {
  UseCounter::count(document(), UseCounter::SVGCursorElementHasClient);

  m_clients.add(element);
  element->setCursorElement(this);
}

void SVGCursorElement::removeReferencedElement(SVGElement* element) {
  m_clients.remove(element);
}

void SVGCursorElement::svgAttributeChanged(const QualifiedName& attrName) {
  if (attrName == SVGNames::xAttr || attrName == SVGNames::yAttr ||
      SVGTests::isKnownAttribute(attrName) ||
      SVGURIReference::isKnownAttribute(attrName)) {
    SVGElement::InvalidationGuard invalidationGuard(this);

    // Any change of a cursor specific attribute triggers this recalc.
    for (const auto& client : m_clients)
      client->setNeedsStyleRecalc(
          LocalStyleChange,
          StyleChangeReasonForTracing::create(StyleChangeReason::SVGCursor));

    return;
  }

  SVGElement::svgAttributeChanged(attrName);
}

DEFINE_TRACE(SVGCursorElement) {
  visitor->trace(m_x);
  visitor->trace(m_y);
  visitor->trace(m_clients);
  SVGElement::trace(visitor);
  SVGTests::trace(visitor);
  SVGURIReference::trace(visitor);
}

}  // namespace blink
