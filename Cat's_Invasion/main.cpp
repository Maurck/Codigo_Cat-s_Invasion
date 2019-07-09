#include <iostream>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;


class Jugador {
	
private:
	
	float x, y;
	float velocidad=3.5;

public:
	
	Texture Tperro;
	Sprite Sperro;
	RenderWindow* ventana;

	Jugador(float x,float y,RenderWindow *ventana)
	{
		this->ventana = ventana;
		this->x = x;
		this->y = y;

		if (!Tperro.loadFromFile("Texturas/doge.png"))
			cout << "No se pudo cargar la textura" << endl;

		Sperro.setTexture(Tperro);
		Sperro.setPosition(x, y);	
		Sperro.setScale(Vector2f(0.05f, 0.05f));
	}

	void update()
	{
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			x-=velocidad;
			Sperro.setPosition(x,y);
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			x+=velocidad;
			Sperro.setPosition(x, y);
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			y-=velocidad;
			Sperro.setPosition(x, y);
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			y+=velocidad;
			Sperro.setPosition(x, y);
		}
	}

	void render()
	{
		this->ventana->draw(this->Sperro);
	}
};

class Nivel
{
public:
	Texture Tfondo;
	Sprite Sfondo;
	RenderWindow* ventana;
	Event evento;
	bool nivel_activo = true;

	Jugador *j;

	Nivel(string ruta_fondo, Vector2u ventana_escala,RenderWindow *ventana,Event evento)
	{
		this->ventana = ventana;
		this->evento = evento;

		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x/Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));
		
		j = new Jugador(10, 10, ventana);
	}

	void loop()
	{
		while (nivel_activo)
		{
			eventos();
			
			update();

			render();
		}
	}

	void update()
	{
		this->j->update();
	}

	void eventos()
	{
		while (this->ventana->pollEvent(this->evento))
		{
			if (this->evento.type == Event::Closed)
			{
				this->ventana->close();
				exit(1);
			}
		}
	}

	void render()
	{
		this->ventana->clear();
		
		this->ventana->draw(this->Sfondo);
		this->j->render();

		this->ventana->display();
	}
};

class Juego
{
	private:

	int altura, anchura;
	RenderWindow *ventana;
	Event evento;
	Nivel *n1;

	public:

	Juego(int altura,int anchura)
	{
		
		this->ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		this->ventana->setFramerateLimit(60);
		
		n1 = new Nivel("Texturas/cuarto.jpg", ventana->getSize(),this->ventana,this->evento);

		loop();
	}

	void loop()
	{
		while (this->ventana->isOpen())
		{
			n1->loop();
		}
	}

	void eventos()
	{
		while (this->ventana->pollEvent(this->evento))
		{
			if (this->evento.type == Event::Closed)
			{
				this->ventana->close();
				exit(1);
			}
		}
	}

	void update()
	{
		this->n1->j->update();
	}
	
	void render()
	{
		this->ventana->clear();

		this->n1->render();
		this->n1->j->render();
		
		this->ventana->display();

	}
};

int main()
{
	Juego j(800, 640);
	
	system("pause");
	return 0;
}