#include "input.h"

// Constructeur
Input::Input() : m_endInput(false)
{
    // Initialisation du tableau de touches
    for(int i=0; i<SDL_NUM_SCANCODES; i++) m_key[i] = false;
}

// Actualisation des �v�nements
void Input::updateEvents()
{
    m_click = false;
        
    // Boucle d'�v�nements
    while(SDL_PollEvent(&m_event))
    {
    
        if(m_event.button.x>0 && m_event.button.x<1248 && m_event.button.y>0 && m_event.button.y<848){
	    	m_x = m_event.button.x;
			m_y = m_event.button.y;
		}
    
        // D�termination des �v�nements actifs
        switch(m_event.type)
        {
                    
            // Clavier actif
            case SDL_KEYDOWN: m_key[m_event.key.keysym.scancode] = true;
            break;
            case SDL_KEYUP: m_key[m_event.key.keysym.scancode] = false;
            break;
            
            case SDL_MOUSEBUTTONDOWN: m_click = true;
            break;
            
            // Fermeture de la fen�tre
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
    // On d�clare le traitement des entr�es termin�
    return m_endInput;
}

bool Input::getKey(const SDL_Scancode key) const
{
    // On renvoie la touche appuy�e
    return m_key[key];
}

pair<int, int> Input::getMouse(){
    return make_pair(m_x, m_y);
}

bool Input::getClick(){
	return m_click;
}
















