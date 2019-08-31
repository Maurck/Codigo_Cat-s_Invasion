#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/audio.hpp>
#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "Collision.hpp"
#include "conio.h"
#include "cstdlib"
#include "fstream"

using namespace std;
using namespace sf;

class Animacion {
private:
	Vector2u numImagenes;
	bool repetirAnim;

	float tiempoTotal;
	float tiempoCambio;
	
public:
	IntRect uvRect;
	bool ultimoCuadro = false;
	Vector2u imagenActual;

	Animacion(Texture* textura, Vector2u numImagenes, float tiempoCambio,bool repetirAnim)
	{
		this->numImagenes = numImagenes;
		this->tiempoCambio = tiempoCambio;
		this->repetirAnim = repetirAnim;
		tiempoTotal = 0.0f;

		uvRect.width = int(textura->getSize().x / numImagenes.x);
		uvRect.height = int(textura->getSize().y / numImagenes.y);
	}

	void siguienteAnim(int fila,float deltaTime,bool faceRight,bool alReves)
	{
		imagenActual.y = fila;
		tiempoTotal += deltaTime;

		if (tiempoTotal >= tiempoCambio)
		{
			tiempoTotal -= tiempoCambio;

			if (alReves)
				imagenActual.x--;
			else
				imagenActual.x++;
		}

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

		if (imagenActual.x == numImagenes.x - 1)
			ultimoCuadro = true;
		else
			ultimoCuadro = false;
	}

	void update(int fila, float deltaTime,bool faceRight,bool alReves)
	{
		if (repetirAnim)
		{
			siguienteAnim(fila,deltaTime,faceRight,alReves);

			if (imagenActual.x >= numImagenes.x)
				imagenActual.x = 0;
		}
		else
		{
			if (imagenActual.x < numImagenes.x - 1)
				siguienteAnim(fila, deltaTime, faceRight,alReves);
		}
	}

	void resetAnim()
	{
		imagenActual.x = 0;
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

	void setString(string mensaje)
	{
		texto.setString(mensaje);
	}
};

class UImenu {

	private:
	
		Texture Tpuntero;
		Sprite Spuntero;
		string textura;
		Vector2f posInicial;
		RenderWindow* ventana;
		int opcion = 1, timer, delay,numOpciones;
		float distancia;
		Vector2f posPunteroinit;

	public:

		Texto* texto[10];

		UImenu(string textura,RenderWindow* ventana,Vector2f posInicial,Vector2f offset, string fuenteTitulo,int tamañoTitulo,Color colorTitulo,string fuenteOpciones,int tamañoOpciones,Color colorOpciones,int numOpciones,string opciones[],float distancia,Vector2f escala,int delay)
		{
			this->textura = textura;
			this->ventana = ventana;
			this->delay = delay;
			this->distancia = distancia;
			this->posInicial = posInicial;
			this->numOpciones = numOpciones;
			Tpuntero.loadFromFile(textura);
			Spuntero.setTexture(Tpuntero);
			Spuntero.setScale(escala);
			timer = delay;

			texto[0] = new Texto(ventana, fuenteTitulo, opciones[0], colorTitulo, tamañoTitulo);
			texto[0]->setPos(posInicial.x, posInicial.y);

			texto[1] = new Texto(ventana, fuenteOpciones, opciones[1], colorOpciones, tamañoOpciones);
			texto[1]->setPos(offset.x, texto[0]->getPos_y() + texto[0]->getSize_y() + offset.y);

			for (int i = 2; i < numOpciones; i++)
			{
				texto[i] = new Texto(ventana, fuenteOpciones, opciones[i], colorOpciones, tamañoOpciones);
				texto[i]->setPos(offset.x , texto[i-1]->getPos_y() + texto[i-1]->getSize_y() + distancia);
			}

			posPunteroinit = Vector2f(texto[1]->getPos_x() - Spuntero.getGlobalBounds().width, texto[1]->getPos_y() + texto[1]->getSize_y() - Spuntero.getGlobalBounds().height / 2);
			
			Spuntero.setPosition(posPunteroinit);
		}

		void update()
		{
			if (timer < delay)
				timer++;
				
			if (Keyboard::isKeyPressed(Keyboard::Down) && timer == delay)
			{
				
				if (opcion == numOpciones - 1)
				{
					Spuntero.setPosition(posPunteroinit);
					opcion = 1;
				}
				else
				{
					Spuntero.move(0.f, texto[1]->getSize_y() +  distancia);
					opcion++;
				}
				timer = 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::Up) && timer == delay)
			{

				if (opcion == 1)
				{
					Spuntero.setPosition(posPunteroinit.x, posPunteroinit.y + (numOpciones - 2) * (texto[1]->getSize_y() + distancia));
					opcion = numOpciones - 1;
				}
				else
				{
					Spuntero.move(0.f, -(texto[1]->getSize_y() + distancia));
					opcion--;
				}
				timer = 0;
			}

		}

		void render()
		{
			this->ventana->draw(this->Spuntero);

			for (int i = 0; i < numOpciones; i++)
			{
				texto[i]->render();
			}
		}

		int getOpc()
		{
			return opcion;
		}
};

class Personaje {
	
private:

	float velocidad;
	float tiempoAnim;
	float deltaTime = 0.0f;
	bool miraDerecha = true;
	Texture Tniño;
	RectangleShape niño;
	Animacion* animacionNiño;
	RenderWindow* ventana;

public:

	Personaje(Vector2f initPos, RenderWindow* ventana, string rutaTextura, Vector2f tamaño, float velocidad)
	{
		this->ventana = ventana;
		this->velocidad = 100.f;
		tiempoAnim = 100 / velocidad * 0.2f;
		Tniño.loadFromFile(rutaTextura);
		niño.setSize(tamaño);
		niño.setTexture(&Tniño);

		animacionNiño = new Animacion(&Tniño, Vector2u(8, 1), tiempoAnim,true);

		niño.setTextureRect(animacionNiño->uvRect);
		niño.setPosition(Vector2f(0.0f,ventana->getSize().y - niño.getSize().y * 2.1f));
	}
	 
	void update(RenderWindow* ventana,float deltaTime)
	{
		Vector2f movimiento(0.0f, 0.0f);

		if (Keyboard::isKeyPressed(Keyboard::Left))
			if(niño.getPosition().x > -10.f)
				movimiento.x -= velocidad * deltaTime;

		if (Keyboard::isKeyPressed(Keyboard::Right))
			if(niño.getPosition().x < ventana->getSize().x)
				movimiento.x += velocidad * deltaTime;

		if (movimiento.x > 0)
			miraDerecha = true;
		else if (movimiento.x < 0)
			miraDerecha = false;

		if(movimiento.x != 0)
		{ 
			animacionNiño->update(0, deltaTime, miraDerecha,false);
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
	float multiplicador = 60.f;
public:
	vector<Enemigo> enemigos;

	Spawner(RenderWindow* ventana, string textura, Vector2f escala, int spawnDelay,float velocidad) : enemigo(ventana,textura,escala)
	{
		this->velocidad = velocidad;
		this->spawnDelay = spawnDelay;
		this->ventana = ventana;
		spawnTimer = this->spawnDelay;
	}

	void update(float deltaTime)
	{
		for (size_t i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].move(Vector2f(-velocidad * deltaTime * multiplicador, 0.0f));

			if (enemigos[i].getPosition().x < -enemigo.getSize().width)
			{
				enemigos.erase(enemigos.begin() + i);
			}
		}
				
		if (spawnTimer <= spawnDelay)
			spawnTimer++;

		if (spawnTimer > spawnDelay)
		{
			enemigo.setPosition(Vector2f(float(ventana->getSize().x),100.f + rand()%int(ventana->getSize().y - enemigo.getSize().height - 200.f)));
			enemigos.push_back(Enemigo(enemigo));
			spawnTimer = 0;
		}

	}

	void render()
	{
		for (size_t i = 0; i < enemigos.size(); i++)
		{
			enemigos[i].render();
		}
	}

};

class UIbar
{
private:
	RenderWindow* ventana;
	RectangleShape cuadrado;
	vector <RectangleShape> barra;
	Vector2f dimension, initPos;
	Texture textura;
	Texto texto;

public:
	int numRectangulos;

	UIbar(RenderWindow* ventana, Vector2f initPos, Vector2f dimension, int numRectangulos,Color colorRelleno,Color colorEsquinas,float tamañoEsquinas,string mensaje,string fuente)
		: texto(ventana,fuente,mensaje,Color::Black,20)
	{
		this->ventana = ventana;
		this->numRectangulos = numRectangulos;
		this->dimension = dimension;
		this->initPos = initPos;

		cuadrado.setSize(Vector2f(dimension.x, dimension.y));
		cuadrado.setOutlineThickness(tamañoEsquinas);
		cuadrado.setFillColor(colorRelleno);
		cuadrado.setOutlineColor(colorEsquinas);

		texto.setPos(cuadrado.getPosition().x, cuadrado.getPosition().y - texto.getSize_y() - 20.f);
		

		for (int i = 0; i < numRectangulos; i++)
		{
			cuadrado.setPosition(Vector2f(cuadrado.getOutlineThickness() + initPos.x + i * (cuadrado.getSize().x + cuadrado.getOutlineThickness()), cuadrado.getOutlineThickness() + initPos.y));
			barra.push_back(RectangleShape(cuadrado));
		}
	}

	UIbar(RenderWindow* ventana, Vector2f initPos, Vector2f dimension, int numRectangulos, string rutaTextura,string mensaje,string fuente)
		: texto(ventana,fuente, mensaje, Color::Black, 45)
	{
		this->ventana = ventana;
		this->numRectangulos = numRectangulos;
		this->dimension = dimension;
		this->initPos = initPos;

		textura.loadFromFile(rutaTextura);
		cuadrado.setTexture(&textura);
		cuadrado.setSize(Vector2f(dimension.x, dimension.y));

		texto.setPos(initPos.x, initPos.y - texto.getSize_y() - 20.f);

		for (int i = 0; i < numRectangulos; i++)
		{
			cuadrado.setPosition(Vector2f(cuadrado.getOutlineThickness() + initPos.x + i * (cuadrado.getSize().x + cuadrado.getOutlineThickness()), cuadrado.getOutlineThickness() + initPos.y));
			barra.push_back(RectangleShape(cuadrado));
		}
	}

	void RectanguloMenos()
	{
		barra.erase(barra.begin() + numRectangulos - 1);
		numRectangulos--;	
	}

	void RectanguloMas()
	{
		cuadrado.setPosition(Vector2f(cuadrado.getOutlineThickness() + initPos.x + numRectangulos * (cuadrado.getSize().x + cuadrado.getOutlineThickness()), cuadrado.getOutlineThickness() + initPos.y));
		barra.push_back(RectangleShape(cuadrado));
		numRectangulos++;
	}

	void render()
	{
		for (int i = 0; i < signed(barra.size()); i++)
		{
			ventana->draw(barra[i]);
		}

		texto.render();
	}

	FloatRect pos()
	{
		return cuadrado.getGlobalBounds();
	}

};

class Bala {

private:
	Texture Tbala;
	RenderWindow* ventana;
public:
	Sprite Sbala;
	Bala(RenderWindow* ventana, string textura, Vector2f escala)
	{
		this->ventana = ventana;
		Tbala.loadFromFile(textura);
		Sbala.setTexture(Tbala);
		Sbala.setScale(escala);
	}

	void render()
	{
		ventana->draw(Sbala);
	}

	void setPosition(Vector2f pos)
	{
		Sbala.setPosition(pos);
	}

	void move(Vector2f mov)
	{
		Sbala.move(mov);
	}

	Vector2f getPosition()
	{
		return Sbala.getPosition();
	}

	FloatRect getSize()
	{
		return Sbala.getGlobalBounds();
	}

};

class Mejora {

private:
	Texture Tmejora;
	RenderWindow* ventana;
public:
	Sprite Smejora;
	Mejora(RenderWindow* ventana, string textura, Vector2f escala)
	{
		this->ventana = ventana;
		Tmejora.loadFromFile(textura);
		Smejora.setTexture(Tmejora);
		Smejora.setScale(escala);
	}

	void render()
	{
		ventana->draw(Smejora);
	}

	void setPosition(Vector2f pos)
	{
		Smejora.setPosition(pos);
	}

	void move(Vector2f mov)
	{
		Smejora.move(mov);
	}

	Vector2f getPosition()
	{
		return Smejora.getPosition();
	}

	FloatRect getSize()
	{
		return Smejora.getGlobalBounds();
	}

};

class SpawnerMejoras {
private:

	Mejora mejora;
	RenderWindow* ventana;
	int spawnTimer, spawnDelay;
	float velocidad;
	float multiplicador = 60.f;
public:
	vector<Mejora> mejoras;

	SpawnerMejoras(RenderWindow* ventana, string textura, Vector2f escala, int spawnDelay, float velocidad) : mejora(ventana, textura, escala)
	{
		this->velocidad = velocidad;
		this->spawnDelay = spawnDelay;
		this->ventana = ventana;
		spawnTimer = this->spawnDelay;
	}

	void update(float deltaTime,bool canSpawn)
	{
		for (size_t i = 0; i < mejoras.size(); i++)
		{
			mejoras[i].move(Vector2f(-velocidad * deltaTime * multiplicador, 0.0f));

			if (mejoras[i].getPosition().x < -mejora.getSize().width)
			{
				mejoras.erase(mejoras.begin() + i);
			}
		}

		if (canSpawn)
		{
			if (spawnTimer <= spawnDelay)
				spawnTimer++;

			if (spawnTimer > spawnDelay)
			{
				mejora.setPosition(Vector2f(float(ventana->getSize().x), 100.f + rand() % int(ventana->getSize().y - mejora.getSize().height - 200.f)));
				mejoras.push_back(Mejora(mejora));
				spawnTimer = 0;
			}
		}
	}

	void mostrarTamañoArreglo()
	{
		cout << mejoras.size() << endl;
	}

	void render()
	{
		for (size_t i = 0; i < mejoras.size(); i++)
		{
			mejoras[i].render();
		}
	}

};

class Jugador {

private:

	float velocidad = 10;
	float deltaTime = 0.0f;
	bool miraDerecha = true;
	Texture Tperro;
	float multiplicador = 60.f;
	Texture TanimPerro;

public:
	Sprite Sperro;
	Animacion* animacionPerro;
	Texture Tshield;
	Sprite Shield;
	int vida = 5;
	bool modoLaser = false;
	Sprite perro;

	Jugador(Vector2f initPos, RenderWindow* ventana)
	{

		Collision::CreateTextureAndBitmask(Tperro, "Texturas/dogeCartoon.png");

		Sperro.setTexture(Tperro);
		Sperro.setPosition(initPos);
		Sperro.setScale(Vector2f(0.1f, 0.1f));

		Tshield.loadFromFile("Texturas/dogeCartoonShield.png");
		Shield.setTexture(Tshield);
		Shield.setScale(0.1f, 0.1f);

		Collision::CreateTextureAndBitmask(TanimPerro, "Texturas/laserAnim.png");
		perro.setTexture(TanimPerro);
		perro.setScale(Vector2f(0.1f, 0.1f));

		animacionPerro = new Animacion(&TanimPerro, Vector2u(8, 1), 0.15f, false);

		perro.setTextureRect(animacionPerro->uvRect);
		perro.setPosition(Vector2f(0.0f, ventana->getSize().y - perro.getGlobalBounds().height * 2.1f));
	}

	void updateColisionesJugador(Spawner& spawn, SpawnerMejoras& spawnMejoras, SpawnerMejoras& comida, UIbar* vidas, UIbar* escudos,Sprite jugador)
	{
		for (size_t i = 0; i < spawn.enemigos.size(); i++)
		{
			if (Collision::PixelPerfectTest(jugador, spawn.enemigos[i].Senemigo))
			{
				spawn.enemigos.erase(spawn.enemigos.begin() + i);

				if (escudos->numRectangulos <= 0)
				{
					vidas->RectanguloMenos();
					vida--;
				}
				else
					escudos->RectanguloMenos();
			}
		}

		for (size_t i = 0; i < spawnMejoras.mejoras.size(); i++)
		{
			if (Collision::PixelPerfectTest(jugador, spawnMejoras.mejoras[i].Smejora))
			{
				spawnMejoras.mejoras.erase(spawnMejoras.mejoras.begin() + i);

				if (vida < 5)
				{
					vidas->RectanguloMas();
					vida++;
				}
			}
		}

		for (size_t i = 0; i < comida.mejoras.size(); i++)
		{
			if (Collision::PixelPerfectTest(jugador, comida.mejoras[i].Smejora))
			{
				comida.mejoras.erase(comida.mejoras.begin() + i);

				if (escudos->numRectangulos < vidas->numRectangulos)
				{
					escudos->RectanguloMas();
				}
			}
		}
	}

	void update(RenderWindow* ventana, Spawner& spawn, SpawnerMejoras& spawnMejoras,SpawnerMejoras& comida, UIbar* vidas,UIbar* escudos, float deltaTime,bool laserMode)
	{
		multiplicador = 60.f;

		if (Keyboard::isKeyPressed(Keyboard::L) && !modoLaser && laserMode)
		{
			perro.setPosition(Vector2f(Sperro.getPosition().x, Sperro.getPosition().y - 25.f));
			animacionPerro->resetAnim();
			modoLaser = true;
		}

		if (!modoLaser)
		{
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				if (Sperro.getPosition().x > 0)
				{
					Sperro.move(-velocidad * deltaTime * multiplicador, 0.0f);
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				if (Sperro.getPosition().x < ventana->getSize().x - Sperro.getGlobalBounds().width)
				{
					Sperro.move(velocidad * deltaTime * multiplicador, 0.0f);
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				if (Sperro.getPosition().y > 100)
				{
					Sperro.move(0.f, -velocidad * deltaTime * multiplicador);
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				if (Sperro.getPosition().y < ventana->getSize().y - Sperro.getGlobalBounds().height - 100)
				{
					Sperro.move(0.f, velocidad * deltaTime * multiplicador);
				}
			}

			updateColisionesJugador(spawn, spawnMejoras, comida, vidas, escudos, Sperro);

			Shield.setPosition(Vector2f(Sperro.getPosition().x - 4.0f, Sperro.getPosition().y - 5.0f));

		}
		else
		{
			Vector2f movimiento(0.0f, 0.0f);

			if (Keyboard::isKeyPressed(Keyboard::A))
				if (perro.getPosition().x > 0)
					movimiento.x -= velocidad * multiplicador * deltaTime;

			if (Keyboard::isKeyPressed(Keyboard::D))
				if (perro.getPosition().x < ventana->getSize().x - perro.getGlobalBounds().width)
					movimiento.x += velocidad * multiplicador * deltaTime;

			if (Keyboard::isKeyPressed(Keyboard::W))
				if (perro.getPosition().y > 80.f)
					movimiento.y -= velocidad * multiplicador * deltaTime;

			if (Keyboard::isKeyPressed(Keyboard::S))
				if (perro.getPosition().y < ventana->getSize().y - perro.getGlobalBounds().height - 100.f)
					movimiento.y += velocidad * multiplicador * deltaTime;

			if(laserMode)
				animacionPerro->update(0, deltaTime, miraDerecha,false);
			else
				animacionPerro->update(0, deltaTime, miraDerecha, true);

			perro.setTextureRect(animacionPerro->uvRect);
			perro.move(movimiento);

			updateColisionesJugador(spawn,spawnMejoras,comida,vidas,escudos, perro);

			Shield.setPosition(Vector2f(perro.getPosition().x - 5.0f, perro.getPosition().y + 20.f));
		}

	}

	void render(RenderWindow* ventana,UIbar* escudos)
	{
		if (!modoLaser)
			ventana->draw(Sperro);
		else
			ventana->draw(perro);

		if (escudos->numRectangulos > 0)
			ventana->draw(Shield);
	}

};

class SpawnerBala {
private:

	Bala bala;
	RenderWindow* ventana;
	int spawnTimer, spawnDelay;
	float velocidad;
	float multiplicador = 60.f;
	Jugador* jugador;
public:
	vector<Bala> balas;
	int puntuacion = 0;

	SpawnerBala(RenderWindow* ventana, Jugador* jugador, string textura, Vector2f escala, int spawnDelay, float velocidad) : bala(ventana, textura, escala)
	{
		this->velocidad = velocidad;
		this->spawnDelay = spawnDelay;
		this->ventana = ventana;
		this->jugador = jugador;
		spawnTimer = this->spawnDelay;
	}

	void update(float deltaTime, Spawner& spawn,bool modoLaser)
	{
		for (size_t i = 0; i < balas.size(); i++)
		{
			balas[i].move(Vector2f(velocidad * deltaTime * multiplicador, 0.0f));

			if (balas[i].getPosition().x > ventana->getSize().x)
			{
				balas.erase(balas.begin() + i);
			}
		}

		if (spawnTimer <= spawnDelay)
			spawnTimer++;

		if (Keyboard::isKeyPressed(Keyboard::Space) && spawnTimer > spawnDelay && jugador->animacionPerro->ultimoCuadro && jugador->modoLaser  && modoLaser)
		{
			bala.setPosition(Vector2f(jugador->perro.getPosition().x + jugador->perro.getGlobalBounds().width / 1.5f, jugador->perro.getPosition().y + (jugador->perro.getGlobalBounds().height / 2.f)));

			balas.push_back(Bala(bala));
			spawnTimer = 0;
		}

		for (int i = 0; i < spawn.enemigos.size(); i++)
		{
			for (int j = 0; j < balas.size(); j++)
			{
				if (Collision::PixelPerfectTest(balas[j].Sbala, spawn.enemigos[i].Senemigo))
				{
					spawn.enemigos.erase(spawn.enemigos.begin() + i);
					balas.erase(balas.begin() + j);
					puntuacion += 100;
					break;

				}
			}
		}

	}

	void render()
	{
		for (size_t i = 0; i < balas.size(); i++)
		{
			balas[i].render();
		}
	}

};


class Nivel
{
public:

	Texture Tfondo;
	Sprite Sfondo;
	Texture TmenuPausa;
	Sprite SmenuPausa;
	RenderWindow* ventana;
	Event evento;
	bool nivel_activo = true;
	float deltaTime = 0.0f;
	Spawner gatos;
	Jugador* perro;
	SpawnerBala* balas;
	SpawnerMejoras huesos;
	SpawnerMejoras comida;
	UImenu* pMenu;
	string opcionesPausa[3] = { "PAUSA","CONTINUAR","SALIR" };
	UIbar* barraVida;
	UIbar* barraEscudo;
	UIbar* barraCargaLaser;
	Music musica;
	int multiplo10 = 1;
	int laserTimer, laserDelay;
	bool modoLaser = false;
	bool pausa = false;
	int velocidadGatos = 20;
	int spawnDelayGatos = 20;

	Texto puntText,puntuacionNum,nivelText;

	Nivel(string ruta_fondo,RenderWindow *ventana,int numNivel,int laserDelay) 
	   :gatos(ventana,"Texturas/cartoon_cat.png",Vector2f(0.2f,0.2f),50,5),
		puntText(ventana,"Fuentes/fuente_cartoon.ttf","Puntuación",Color::Black,40),
		puntuacionNum(ventana, "Fuentes/fuente_cartoon.ttf", "0", Color::Black, 40),
		nivelText(ventana, "Fuentes/fuente_cartoon.ttf", "Nivel  " + to_string(numNivel), Color::Black, 50),
		huesos(ventana, "Texturas/hueso.png", Vector2f(0.2f, 0.2f), 200, 10),
		comida(ventana, "Texturas/comida.png", Vector2f(0.2f, 0.2f), 100, 10)
	{
		this->ventana = ventana;
		this->evento = evento;
		this->laserDelay = laserDelay;

		laserTimer = laserDelay;

		TmenuPausa.loadFromFile("Texturas/menuPausa.png");
		SmenuPausa.setTexture(TmenuPausa);
		SmenuPausa.setScale(Vector2f(0.6f, 1.f));
		SmenuPausa.setColor(Color(255,255,255,180));
	

		SmenuPausa.setPosition(Vector2f(ventana->getSize().x / 2 - SmenuPausa.getGlobalBounds().width / 2, ventana->getSize().y / 2 - SmenuPausa.getGlobalBounds().height / 2));

		pMenu = new UImenu("Texturas/apuntador.png", ventana, Vector2f(SmenuPausa.getPosition().x + 50.f, SmenuPausa.getPosition().y + 30.f),Vector2f(300.f,100.f),"Fuentes/fuente_bonita.ttf",45,Color::Magenta,"Fuentes/fuente_bonita.ttf ",30,Color::Black, 3,opcionesPausa, 40.f, Vector2f(0.3f, 0.3f), 20);

		puntText.setPos(80.f,5.f);
		puntuacionNum.setPos(puntText.getPos_x() + puntText.getSize_x() / 2 - puntuacionNum.getSize_x(), puntText.getPos_y() + 35.f);

		nivelText.setPos(puntText.getPos_x() + puntText.getSize_x() + 100.f, nivelText.getPos_y() + 10.f);

		perro = new Jugador(Vector2f(40.f, ventana->getSize() .y / 2.f), ventana);
		balas = new SpawnerBala(ventana, perro, "Texturas/laser.png", Vector2f(0.35f, 0.35f), 10, 10);
		barraVida = new UIbar(ventana,Vector2f(20.0f, ventana->getSize().y - 45.0f), Vector2f(50.f, 40.f), perro->vida,"Texturas/heart2.png","Vidas:","Fuentes/fuente_cartoon.ttf");
		barraEscudo = new UIbar(ventana, Vector2f(20.0f, ventana->getSize().y - 45.0f), Vector2f(50.f, 40.f),0, "Texturas/escudo.png", "", "Fuentes/fuente_cartoon.ttf");
		barraCargaLaser = new UIbar(ventana, Vector2f(420.f, ventana->getSize().y - 45.0f), Vector2f(50.f, 40.f), 0, "Texturas/thunder.png", "Carga de modo laser:", "Fuentes/fuente_cartoon.ttf");
		
		Tfondo.loadFromFile(ruta_fondo);
		Sfondo.setTexture(Tfondo);
		Sfondo.setScale(Vector2f(ventana->getSize().x /Sfondo.getGlobalBounds().width, ventana->getSize().y / Sfondo.getGlobalBounds().height));

		musica.openFromFile("Musica/nivelMusica.ogg");
		musica.setLoop(true);
	}

	void loop()
	{
		
		Clock reloj;
		srand(time(NULL));
		//musica.play();

		while (nivel_activo && perro->vida > 0)
		{
			deltaTime = reloj.restart().asSeconds();
			
			if (!pausa)
			
				update();
			else
			{
				pMenu->update();

				if (Keyboard::isKeyPressed(Keyboard::Enter))
				{
					switch (pMenu->getOpc())
					{
					case 1: pausa = false; break;
					case 2: pausa = false; nivel_activo = false; break;
					}
				}
			}
			
			render();
			
			eventos();
		}
	}

	void updatePuntuacion()
	{
		if (balas->puntuacion != 0)
		{
			if (balas->puntuacion % multiplo10 == 0)
			{
				puntuacionNum.setPos(puntuacionNum.getPos_x() - 5.f, puntuacionNum.getPos_y());
				multiplo10 *= 10;
			}
		}

		puntuacionNum.setString(to_string(balas->puntuacion));
	}

	void updateModoLaser()
	{
		if (laserTimer <= laserDelay)
			laserTimer++;

		if (laserTimer > laserDelay && barraCargaLaser->numRectangulos < 7 && !modoLaser)
		{
			barraCargaLaser->RectanguloMas();
			laserTimer = 0;

			if (barraCargaLaser->numRectangulos == 7)
				modoLaser = true;
		}

		if (perro->animacionPerro->ultimoCuadro && modoLaser && laserTimer > laserDelay)
		{
			barraCargaLaser->RectanguloMenos();
			laserTimer = 0;

			if (barraCargaLaser->numRectangulos == 0)
			{
				modoLaser = false;
				perro->modoLaser = false;
				perro->animacionPerro->ultimoCuadro = false;
				perro->Sperro.setPosition(Vector2f(perro->perro.getPosition().x, perro->perro.getPosition().y + 25.f));
			}
		}
	}

	void update()
	{
		perro->update(ventana, gatos, huesos,comida, barraVida,barraEscudo,deltaTime, modoLaser);
		balas->update(deltaTime, gatos, modoLaser);

		huesos.update(deltaTime,barraVida->numRectangulos < 5);
		comida.update(deltaTime,barraEscudo->numRectangulos < barraVida->numRectangulos);
		
		gatos.update(deltaTime);

		updatePuntuacion();
		updateModoLaser();
		
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

			if (evento.key.code == Keyboard::P)
				pausa = true;

			if (evento.key.code == Keyboard::O)
				pausa = false;
		}
	}

	void render()
	{
		ventana->clear();
		
		ventana->draw(Sfondo);

		perro->render(ventana,barraEscudo);
		gatos.render();
		balas->render();
		huesos.render();
		comida.render();
		barraVida->render();
		barraEscudo->render();
		barraCargaLaser->render();

		puntText.render();
		puntuacionNum.render();

		nivelText.render();

		if (pausa)
		{
			ventana->draw(SmenuPausa);
			pMenu->render();
		}
		
		ventana->display();
	}
};

class Escena
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

	Escena(string ruta_fondo, Vector2u ventana_escala, RenderWindow* ventana, Event evento) :
		historia(ventana, "Fuentes/fuente_elegante.ttf", "",Color::Black, 80)
		, niño(Vector2f(40.f, 80.f), ventana, "Texturas/niñoAnim.png",Vector2f(100.f, 150.f),100.f)
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

		string opciones[3] = { "Cat's Invasion","Jugar","Salir" };

		Texture TdogeMenu;
		Sprite SdogeMenu;
		Texture TcatMenu;
		Sprite ScatMenu;

	public:
		UImenu* p1;

		Menu(RenderWindow *ventana, Vector2u ventana_escala, Event evento)
		{
			this->ventana = ventana;
			this->evento = evento;
			Tmenu.loadFromFile("Texturas/menu.jpg");
			Smenu.setTexture(Tmenu);
			Smenu.setScale(Vector2f(ventana_escala.x / Smenu.getGlobalBounds().width, ventana_escala.y / Smenu.getGlobalBounds().height));


			TdogeMenu.loadFromFile("Texturas/dogeMenu.png");
			SdogeMenu.setTexture(TdogeMenu);
			SdogeMenu.setScale(0.2f, 0.2f);			

			TcatMenu.loadFromFile("Texturas/catMenu.png");
			ScatMenu.setTexture(TcatMenu);
			ScatMenu.setScale(SdogeMenu.getGlobalBounds().width / ScatMenu.getGlobalBounds().width, SdogeMenu.getGlobalBounds().height / ScatMenu.getGlobalBounds().height);

			p1 = new UImenu("Texturas/apuntador.png", ventana, Vector2f(100.f,100.f),Vector2f(250.f,60.f),"Fuentes/fuente_titulo.ttf",100,Color::Blue,"Fuentes/fuente_bonita.ttf",80,Color::Black,3,opciones ,30.f, Vector2f(0.4f, 0.4f), 20);

			SdogeMenu.setPosition(p1->texto[0]->getPos_x() + p1->texto[0]->getSize_x() * 1.01f, p1->texto[0]->getPos_y() + p1->texto[0]->getSize_y() - this->SdogeMenu.getGlobalBounds().height / 1.5f);
			ScatMenu.setPosition(p1->texto[0]->getPos_x() - ScatMenu.getGlobalBounds().width * 1.1f, p1->texto[0]->getPos_y() + p1->texto[0]->getSize_y() - this->ScatMenu.getGlobalBounds().height / 1.5f);
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
			ventana->clear();

			ventana->draw(Smenu);
			ventana->draw(SdogeMenu);
			ventana->draw(ScatMenu);
			p1->render();

			ventana->display();
			
		}
};

class Juego
{
	private:

	int altura, anchura;
	RenderWindow *ventana;
	Event evento;
	Escena *introduccion;
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
		
		introduccion = new Escena("Texturas/cartoon_city.jpg", ventana->getSize(),ventana,evento);
		n1 = new Nivel("Texturas/cartoon_room.jpg",ventana,1,50);
		menu = new Menu(ventana, ventana->getSize(), evento);

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

			delete n1;
			n1 = new Nivel("Texturas/cartoon_room.jpg", ventana, 1, 50);
		}
	}
};

int main()
{
	Juego j(800, 640);

	return 0;
}