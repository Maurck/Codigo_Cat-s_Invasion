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
	int velocidad=3.5;

public:
	
	Texture Tperro;
	Sprite Sperro;

	Jugador(float x,float y)
	{
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
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			x+=velocidad;
			Sperro.setPosition(x,y);
		}

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			x-=velocidad;
			Sperro.setPosition(x, y);
		}

		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			y-=velocidad;
			Sperro.setPosition(x, y);
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			y+=velocidad;
			Sperro.setPosition(x, y);
		}
	}
};

class Juego
{
	private:

	int altura, anchura;
	RenderWindow *ventana;
	Jugador j = Jugador(10,10);
	Event evento;

	public:

	Juego(int altura,int anchura)
	{
		this->ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		this->ventana->setFramerateLimit(60);

		loop();
	}

	void loop()
	{
		while (this->ventana->isOpen())
		{
			eventos();

			update();
			
			render();
		}
	}

	void eventos()
	{
		while (this->ventana->pollEvent(this->evento))
		{
			if (this->evento.type == Event::Closed)
				this->ventana->close();
		}
	}

	void update()
	{
		this->j.update();
	}
	
	void render()
	{
		this->ventana->clear();

		this->ventana->draw(this->j.Sperro);
		
		this->ventana->display();

	}
};

int main()
{
	Juego j(800, 640);
	
	system("pause");
	return 0;
}