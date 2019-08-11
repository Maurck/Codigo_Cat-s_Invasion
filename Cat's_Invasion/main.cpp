#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>

using namespace std;
using namespace sf;

class Animacion {
private:
	Vector2u numImagenes;
	Vector2u imagenActual;

	float tiempoTotal;
	float tiempoCambio;

public:
	IntRect uvRect;

	Animacion(Texture* textura, Vector2u numImagenes, float tiempoCambio)
	{
		this->numImagenes = numImagenes;
		this->tiempoCambio = tiempoCambio;
		tiempoTotal = 0.0f;

		imagenActual.x = 0;

		uvRect.width = textura->getSize().x / float(numImagenes.x);
		uvRect.height = textura->getSize().y / float(numImagenes.y);
	}

	void update(int fila, float deltaTime,bool faceRight)
	{
		imagenActual.y = fila;
		tiempoTotal += deltaTime;

		if (tiempoTotal >= tiempoCambio)
		{
			tiempoTotal -= tiempoCambio;
			imagenActual.x++;
		}

		if (imagenActual.x >= numImagenes.x)
			imagenActual.x = 0;

		uvRect.top = imagenActual.y * uvRect.height;
		
		if (faceRight)
		{
			uvRect.left = imagenActual.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else
		{
			uvRect.left = (imagenActual.x + 1) * abs(uvRect.width);
			uvRect.width = -abs(uvRect.width);
		}

	}
};


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
		int opcion = 1, timer, delay;
		float distancia;

	public:
		Puntero(string textura,RenderWindow* ventana,float distancia,Vector2f escala,int delay)
		{
			this->textura = textura;
			this->ventana = ventana;
			this->delay = delay;
			this->distancia = distancia;
			Tpuntero.loadFromFile(textura);
			Spuntero.setTexture(Tpuntero);
			Spuntero.setScale(escala);
			timer = delay;
		}

		void update()
		{
			if (timer < delay)
				timer++;
				
			if (Keyboard::isKeyPressed(Keyboard::Down) && timer == delay)
			{
				
				if (opcion == 2)
				{
					Spuntero.setPosition(x, y);
					opcion = 1;
				}
				else
				{
					Spuntero.move(0.f, distancia);
					opcion++;
				}
				timer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up) && timer == delay)
			{

				if (opcion == 1)
				{
					Spuntero.setPosition(x, y + distancia);
					opcion = 2;
				}
				else
				{
					Spuntero.move(0.f, -distancia);
					opcion--;
				}
				timer = 0;
			}

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

class Personaje {
	
private:

	float velocidad=500;
	float deltaTime = 0.0f;
	bool miraDerecha = true;
	Texture Tniño;
	RectangleShape niño;
	Animacion* animacionNiño;
	float velocidadAnim = 0.15f;
	RenderWindow* ventana;

public:

	Personaje(Vector2f initPos,RenderWindow* ventana)
	{
		this->ventana = ventana;
		Tniño.loadFromFile("Texturas/niñoAnim.png");
		niño.setSize(Vector2f(100.f, 150.f));
		niño.setTexture(&Tniño);

		animacionNiño = new Animacion(&Tniño, Vector2u(8, 1), velocidadAnim);

		niño.setTextureRect(animacionNiño->uvRect);
		niño.setPosition(Vector2f(0.0f,ventana->getSize().y - niño.getSize().y * 2.1));
	}

	void update(RenderWindow* ventana,float deltaTime)
	{
		Vector2f movimiento(0.0f, 0.0f);

		if (Keyboard::isKeyPressed(Keyboard::Left))
			movimiento.x -= velocidad * deltaTime;

		if (Keyboard::isKeyPressed(Keyboard::Right))
			movimiento.x += velocidad * deltaTime;

		if (movimiento.x > 0)
			miraDerecha = true;
		else if (movimiento.x < 0)
			miraDerecha = false;

		if(movimiento.x != 0)
		{ 
			animacionNiño->update(0, deltaTime, miraDerecha);
			niño.setTextureRect(animacionNiño->uvRect);
			niño.move(movimiento);
		}
	}

	void render(RenderWindow* ventana)
	{
		ventana->draw(niño);
	}


	bool salioDePantalla()
	{
		if (niño.getPosition().x > ventana->getSize().x)
			return true;

		return false;
	}

};

class Jugador {

private:

	float velocidad = 7;
	float deltaTime = 0.0f;
	bool miraDerecha = true;
	Texture Tperro;
	
public:
	Sprite Sperro;
	int vida = 3;

	Jugador(Vector2f initPos, RenderWindow* ventana)
	{

		if (!Tperro.loadFromFile("Texturas/dogeCartoon.png"))
			cout << "No se pudo cargar la textura" << endl;

		Sperro.setTexture(Tperro);
		Sperro.setPosition(initPos);
		Sperro.setScale(Vector2f(0.1f, 0.1f));
	}

	void update(RenderWindow* ventana,float deltaTime)
	{

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (Sperro.getPosition().x > 0)
			{
				Sperro.move(-velocidad, 0.0f);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (Sperro.getPosition().x < ventana->getSize().x - Sperro.getGlobalBounds().width)
			{
				Sperro.move(velocidad, 0.0f);
			}
		}
			
		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (Sperro.getPosition().y > 0)
			{
				Sperro.move(0.f, -velocidad);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (Sperro.getPosition().y < ventana->getSize().y - Sperro.getGlobalBounds().height)
			{
				Sperro.move(0.f, velocidad);
			}
		}

	}

	void render(RenderWindow* ventana)
	{
		ventana->draw(Sperro);

	}

};

class Enemigo {

private:
	Texture Tenemigo;
	RenderWindow* ventana;
public:
	Sprite Senemigo;
	Enemigo(RenderWindow* ventana, string textura, Vector2f escala)
	{
		this->ventana = ventana;
		Tenemigo.loadFromFile(textura);
		Senemigo.setTexture(Tenemigo);
		Senemigo.setScale(escala);
	}

	void render()
	{
		ventana->draw(Senemigo);
	}

	void setPosition(Vector2f pos)
	{
		Senemigo.setPosition(pos);
	}

	void move(Vector2f mov)
	{
		Senemigo.move(mov);
	}

	Vector2f getPosition()
	{
		return Senemigo.getPosition();
	}

	FloatRect getSize()
	{
		return Senemigo.getGlobalBounds();
	}

};

class Spawner {
private:
	
	Enemigo enemigo;
	RenderWindow* ventana;
	int spawnTimer,spawnDelay;
	float velocidad;

public:
	vector<Enemigo> enemigos;

	Spawner(RenderWindow* ventana, string textura, Vector2f escala, int spawnDelay,float velocidad) : enemigo(ventana,textura,escala)
	{
		this->velocidad = velocidad;
		this->spawnDelay = spawnDelay;
		this->ventana = ventana;
		spawnTimer = this->spawnDelay;
	}

	void update(Jugador &jugador)
	{
		for (size_t i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].move(Vector2f(-velocidad, 0.0f));

			if (enemigos[i].getPosition().x < -enemigo.getSize().width)
			{
				enemigos.erase(enemigos.begin() + i);
			}
		}
		cout << enemigos.size() << endl;
		
		
		if (spawnTimer <= spawnDelay)
			spawnTimer++;

		if (spawnTimer > spawnDelay)
		{
			enemigo.setPosition(Vector2f(ventana->getSize().x,rand()%int(ventana->getSize().y - enemigo.getSize().height)));
			enemigos.push_back(Enemigo(enemigo));
			spawnTimer = 0;
		}

		for (size_t i = 0; i < enemigos.size(); i++)
		{
			if (jugador.Sperro.getGlobalBounds().intersects(enemigos[i].Senemigo.getGlobalBounds()))
			{
				enemigos.erase(enemigos.begin() + i);
				jugador.vida--;
			}
		}

		cout << jugador.vida;
	}

	void render()
	{
		for (size_t i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].render();
		}
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
	float deltaTime = 0.0f;
	Spawner gatos;
	Jugador perro;

	Nivel(string ruta_fondo, Vector2u ventana_escala,RenderWindow *ventana,Event evento) 
		: gatos(ventana,"Texturas/cartoon_cat.png",Vector2f(0.2f,0.2f),50,5) , perro(Vector2f(40.f, 80.f), ventana)
	{
		this->ventana = ventana;
		this->evento = evento;

		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x/Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));
	
	}

	void loop()
	{
		Clock reloj;
		srand(time(NULL));
		perro.vida = 3;

		while (nivel_activo && perro.vida > 0)
		{
			deltaTime = reloj.restart().asSeconds();

			eventos();
			
			update();

			render();
		}
	}

	void update()
	{
		perro.update(ventana,deltaTime);

		gatos.update(perro);
	}

	void eventos()
	{
		while (ventana->pollEvent(evento))
		{
			if (evento.type == Event::Closed)
			{
				ventana->close();
				exit(1);
			}
		}
	}

	void render()
	{
		ventana->clear();
		
		ventana->draw(Sfondo);
		perro.render(ventana);
		gatos.render();
		ventana->display();
	}
};

class Intro
{
public:
	Texture Tfondo;
	Sprite Sfondo;

	RenderWindow* ventana;

	Event evento;

	Texto historia;

	bool intro_activo = true;
	float deltaTime = 0.0f;

	Personaje niño;

	Intro(string ruta_fondo, Vector2u ventana_escala, RenderWindow* ventana, Event evento) :
		historia(ventana, "Fuentes/fuente_elegante.ttf", "Esta es la historia de billy, \nun niño que solo queria ser feliz",Color::Black, 80) 
		, niño(Vector2f(40.f, 80.f), ventana)
	{
		this->ventana = ventana;
		this->evento = evento;

		historia.setPos(50.f, 50.f);

		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana_escala.x / Sfondo.getGlobalBounds().width, ventana_escala.y / Sfondo.getGlobalBounds().height));

	}

	void loop()
	{
		Clock reloj;

		while (intro_activo)
		{
			deltaTime = reloj.restart().asSeconds();

			eventos();

			update();

			render();
		}
	}

	void update()
	{
		niño.update(ventana,deltaTime);

		if (niño.salioDePantalla())
			intro_activo = false;
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
		ventana->clear();

		ventana->draw(this->Sfondo);
		niño.render(ventana);
		historia.render();

		ventana->display();
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

		Texture TdogeMenu;
		Sprite SdogeMenu;

	public:
		Puntero* p1;

		Menu(RenderWindow *ventana, Vector2u ventana_escala, Event evento)
		{
			this->ventana = ventana;
			this->evento = evento;
			Tmenu.loadFromFile("Texturas/menu.jpg");
			Smenu.setTexture(Tmenu);
			Smenu.setScale(Vector2f(ventana_escala.x / Smenu.getGlobalBounds().width, ventana_escala.y / Smenu.getGlobalBounds().height));

			cats_invasion = new Texto(this->ventana, "Fuentes/fuente_titulo.ttf", "Cat's Invasion", Color::Blue,100);
			cats_invasion->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f, this->ventana->getSize().y / 6.f);

			TdogeMenu.loadFromFile("Texturas/dogeMenu.png");
			SdogeMenu.setTexture(TdogeMenu);
			SdogeMenu.setScale(0.2f, 0.2f);			
			SdogeMenu.setPosition(cats_invasion->getPos_x() + cats_invasion->getSize_x()*1.01, cats_invasion->getPos_y() + this->cats_invasion->getSize_y() - this->SdogeMenu.getGlobalBounds().height / 2);

			jugar = new Texto(this->ventana, "Fuentes/fuente_elegante.ttf", "Jugar", Color::Black, 80);
			jugar->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->cats_invasion->getPos_y() + this->cats_invasion->getSize_y() + 50);

			salir = new Texto(this->ventana, "Fuentes/fuente_elegante.ttf", "Salir", Color::Black, 80);
			salir->setPos(this->ventana->getSize().x / 2.f - cats_invasion->getSize_x() / 2.f + 50.f, this->ventana->getSize().y / 6.f + 200.f);

			p1 = new Puntero("Texturas/apuntador.png",ventana, (salir->getPos_y() + (salir->getSize_y() / 2) - (jugar->getPos_y() + jugar->getSize_y() / 2)), Vector2f(0.3, 0.3), 20);
			p1->setPos(jugar->getPos_x() - p1->getSize_x(), jugar->getPos_y() + jugar->getSize_y() - p1->getSize_y() / 2);
			p1->setInitPos(jugar->getPos_x() - p1->getSize_x(), jugar->getPos_y() + jugar->getSize_y() - p1->getSize_y() / 2);
		}

		void loop()
		{
			menu_activo = true;

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
			this->ventana->draw(this->SdogeMenu);
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
	Intro *introduccion;
	Nivel* n1;
	Menu *menu;
	bool jugando = true;

	public:

	Juego(int altura,int anchura)
	{
		
		ventana = new RenderWindow(VideoMode(altura, anchura), "Cat's Invasion");
		ventana->setFramerateLimit(60);
		ventana->setPosition(Vector2i(250, 50));
		ventana->setVerticalSyncEnabled(true);
		
		introduccion = new Intro("Texturas/cartoon_city.jpg", ventana->getSize(),this->ventana,this->evento);
		n1 = new Nivel("Texturas/cartoon_room.jpg", ventana->getSize(), this->ventana, this->evento);
		menu = new Menu(this->ventana, ventana->getSize(), this->evento);

		loop();
	}

	void loop()
	{
		
		while (ventana->isOpen() && jugando)
		{
			menu->loop();

			switch (menu->p1->getOpc())
			{
				case 1:
					//introduccion->loop();
					n1->loop();
					break;
				case 2:
					ventana->close();
					jugando = false;
					break;
				default:
					break;
			}
		}
	}
};

int main()
{
	Juego j(800, 640);

	return 0;
}