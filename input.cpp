#include "input.h"

// Constructeur
Input::Input() : m_endInput(false)
{
    // Initialisation du tableau de touches
    for(int i=0; i<SDL_NUM_SCANCODES; i++) m_key[i] = false;
}

// Actualisation des événements
void Input::updateEvents()
{
    m_click = false;
        
    // Boucle d'évènements
    while(SDL_PollEvent(&m_event))
    {
    
        if(m_event.button.x>0 && m_event.button.x<1248 && m_event.button.y>0 && m_event.button.y<848){
	    	m_x = m_event.button.x;
			m_y = m_event.button.y;
		}
    
        // Détermination des évènements actifs
        switch(m_event.type)
        {
                    
            // Clavier actif
            case SDL_KEYDOWN: m_key[m_event.key.keysym.scancode] = true;
            break;
            case SDL_KEYUP: m_key[m_event.key.keysym.scancode] = false;
            break;
            
            case SDL_MOUSEBUTTONDOWN: m_click = true;
            break;
            
            // Fermeture de la fenêtre
            case SDL_WINDOWEVENT: if(m_event.window.event == SDL_WINDOWEVENT_CLOSE)
            m_endInput = true;
            break;

            // Autre
            default:break;
        }
    }
}

bool Input::endInput() const
{
    // On déclare le traitement des entrées terminé
    return m_endInput;
}

bool Input::getKey(const SDL_Scancode key) const
{
    // On renvoie la touche appuyée
    return m_key[key];
}

pair<int, int> Input::getMouse(){
    return make_pair(m_x, m_y);
}

bool Input::getClick(){
	return m_click;
}
















