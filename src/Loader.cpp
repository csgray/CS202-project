#include "Loader.h"

#include <fstream>
#include <stdexcept>

bool load_texture(sf::Texture& tex,const std::string& filename)
{
    char buffer;
	std::ifstream istr(filename.c_str(),std::ios_base::in|std::ios_base::binary);
	istr.unsetf(std::ios_base::skipws);

	if(!istr)
        throw std::runtime_error("Couldn't open file \""+filename+"\".");

	std::string data="";

	while(istr>>buffer)
		data+=buffer;

	istr.close();
	tex.loadFromMemory(data.c_str(),data.size());
	return true;
}

bool load_font(sf::Font& tex,const std::string& filename)
{
    char buffer;
	std::ifstream istr(filename.c_str(),std::ios_base::in|std::ios_base::binary);
	istr.unsetf(std::ios_base::skipws);

	if(!istr)
        throw std::runtime_error("Couldn't open file \""+filename+"\".");

	std::string data="";

	while(istr>>buffer)
		data+=buffer;

	istr.close();
	tex.loadFromMemory(data.c_str(),data.size());
	return true;
}

bool load_buffer(sf::SoundBuffer& soundBuffer,const std::string& filename)
{
    char buffer;
	std::ifstream istr(filename.c_str(),std::ios_base::in|std::ios_base::binary);
	istr.unsetf(std::ios_base::skipws);

	if(!istr)
        throw std::runtime_error("Couldn't open file \""+filename+"\".");

	std::string data="";

	while(istr>>buffer)
		data+=buffer;

	istr.close();
	soundBuffer.loadFromMemory(data.c_str(),data.size());
	return true;
}
