#include <iostream>
#include <SFML/System.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;


class Texto{

private:
	Font fuente;
	Text texto;
	RenderWindow* ventana;
	int tamaño;
	string mensaje;
	string fuentes;
	float pos_x, pos_y;
	Color color;

public:
	Texto(RenderWindow* ventana,string fuentes,string mensaje,Color color,int tamaño)
	{
		this->ventana = ventana;
		this->mensaje = mensaje;
		this->tamaño = tamaño;
		this->fuente = fuente;
		this->color = color;

		fuente.loadFromFile(fuentes);
		texto.setFont(fuente);
		texto.setCharacterSize(tamaño);
		texto.setString(mensaje);
		texto.setFillColor(color);
	}

	float getSize_x()
	{
		return this->texto.getGlobalBounds().width;
	}

	float getSize_y()
	{
		return this->texto.getGlobalBounds().height;

	}

	float getPos_x()
	{
		return this->texto.getPosition().x;
	}

	float getPos_y()
	{
		return this->texto.getPosition().y;
	}

	void setPos(float x, float y)
	{
		texto.setPosition(x, y);
	}

	void render()
	{
		this->ventana->draw(this->texto);
	}
};

class Puntero {

	private:
		float x, y;
		Texture Tpuntero;
		Sprite Spuntero;
		string textura;
		RenderWindow* ventana;
		int opcion = 1, timer, delay = 80;

	public:
		Puntero(string textura,RenderWindow* ventana)
		{
			this->textura = textura;
			this->ventana = ventana;
			Tpuntero.loadFromFile(textura);
			Spuntero.setTexture(Tpuntero);
			Spuntero.setScale(Vector2f(0.2, 0.2));
			timer = delay;
		}

		void update()
		{
			if (timer < delay)
				timer++;
				
			if (Keyboard::isKeyPressed(Keyboard::S) && timer == delay)
			{
				
				if (opcion == 2)
				{
					Spuntero.setPosition(x, y);
					opcion = 1;
				}
				else
				{
					Spuntero.move(0.f, 50.f);
					opcion++;
				}
				timer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::W) && timer == delay)
			{

				if (opcion == 1)
				{
					Spuntero.setPosition(x, y + 50);
					opcion = 2;
				}
				else
				{
					Spuntero.move(0.f, -50.f);
					opcion--;
				}
				timer = 0;
			}

			cout << opcion << endl;
		}

		void render()
		{
			this->ventana->draw(this->Spuntero);
		}

		float getSize_x()
		{
			return this->Spuntero.getGlobalBounds().width;
		}

		float getSize_y()
		{
			return this->Spuntero.getGlobalBounds().height;

		}
		
		void setPos(float x, float y)
		{
			Spuntero.setPosition(x, y);
		}

		void setInitPos(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		int getOpc()
		{
			return this->opcion;
		}
};

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
		
		j = new Jugador(40, 80, ventana);
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

class Menu
{
	private:

		RenderWindow* ventana;
		Event evento;
		Texture Tmenu;
		Sprite Smenu;
		bool menu_activo = true;
		Texto *cats_invasion,*jugar,*salir;

	public:
		Puntero* p1;

		Menu(RenderWindow *ventana, Vector2u ventana_escala, Event evento)
		{
			this->ventana = ventana;
			Tmenu.loadFromFile("Texturas/menu.jpg");
			Smenu.setTexture(Tmenu);
			Smenu.setScale(Vector2f(ventana_escala.x / Smenu.getGlobalBounds().width, ventana_escala.y / Smenu.getGlobalBounds().height));

			cats_invasion = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "Cat's Invasion", Color::Blue, 50);
			cats_invasion->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f, this->ventana->getSize().y / 6.f);

			jugar = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "1.Jugar", Color::Black, 40);
			jugar->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 100.f);

			p1 = new Puntero("Texturas/apuntador.png", this->ventana);
			p1->setPos(this->jugar->getPos_x() - p1->getSize_x(), this->jugar->getPos_y() + this->jugar->getSize_y() / 4);
			p1->setInitPos(this->jugar->getPos_x() - p1->getSize_x(), this->jugar->getPos_y() + this->jugar->getSize_y() / 4);

			salir = new Texto(this->ventana, "Fuentes/FuenteNormal.ttf", "2.Salir", Color::Black, 40);
			salir->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 150.f);
		}

		void loop()
		{
			while (menu_activo)
			{
				eventos();

				update();

				render();
			}

		}

		void update()
		{
			if (Keyboard::isKeyPressed(Keyboard::Enter))
				menu_activo = false;

			this->p1->update();
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

			this->ventana->draw(this->Smenu);
			this->cats_invasion->render();
			this->p1->render();
			this->jugar->render();
			this->salir->render();

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
	Menu *menu;
	bool jugando = true;

	public:

	Juego(int altura,int anchura)
	{
		
		this->ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		this->ventana->setFramerateLimit(60);
		
		n1 = new Nivel("Texturas/cuarto.jpg", ventana->getSize(),this->ventana,this->evento);
		menu = new Menu(this->ventana, ventana->getSize(), this->evento);

		loop();
	}

	void loop()
	{
		
		while (this->ventana->isOpen() && jugando)
		{
			menu->loop();

			switch (this->menu->p1->getOpc())
			{
				case 1:
					n1->loop();
					break;
				case 2:
					this->ventana->close();
					jugando = false;
					break;
				default:
					break;
			}
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

	return 0;
}