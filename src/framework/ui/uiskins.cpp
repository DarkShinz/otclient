/* The MIT License
 *
 * Copyright (c) 2010 OTClient, https://github.com/edubart/otclient
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "uiskins.h"
#include "../resources.h"
#include "../textures.h"
#include "uielementskin.h"

UISkins g_uiSkins;

UISkins::~UISkins()
{
    for(auto it = m_elementSkins.begin(); it != m_elementSkins.end(); ++it)
        delete (*it);
}

bool UISkins::load(const std::string& file)
{
    std::string fileContents = g_resources.loadTextFile(file);
    if(!fileContents.size()) {
        logFatal("Could not load skin file \"%s",  file.c_str());
        return false;
    }

    std::istringstream fin(fileContents);

    try {
        YAML::Parser parser(fin);

        YAML::Node doc;
        parser.GetNextDocument(doc);

        std::string defaultTexture;
        doc["default skin image"] >> defaultTexture;
        m_defaultTexture = g_textures.get("skins/" + defaultTexture);

        {
            const YAML::Node& node = doc["buttons"];
            for(auto it = node.begin(); it != node.end(); ++it) {
                std::string name;
                it.first() >> name;

                UIElementSkin *elementSkin = new UIElementSkin(name, UI::Button);
                elementSkin->load(it.second());
                m_elementSkins.push_back(elementSkin);
            }
        }

        {
            const YAML::Node& node = doc["panels"];
            for(auto it = node.begin(); it != node.end(); ++it) {
                std::string name;
                it.first() >> name;

                UIElementSkin *elementSkin = new UIElementSkin(name, UI::Panel);
                elementSkin->load(it.second());
                m_elementSkins.push_back(elementSkin);
            }
        }
    } catch (YAML::ParserException& e) {
        logError("Malformed font file \"%s\"", file.c_str());
        return false;
    }
    return true;
}

UIElementSkin* UISkins::getElementSkin(UI::EElementType elementType, const std::string& name)
{
    for(auto it = m_elementSkins.begin(); it != m_elementSkins.end(); ++it) {
        UIElementSkin *skin = (*it);
        if(elementType == skin->getElementType() && name == skin->getName()) {
            return skin;
        }
    }
    logWarning("Element skin '%s' not found", name.c_str());
    return NULL;
}
