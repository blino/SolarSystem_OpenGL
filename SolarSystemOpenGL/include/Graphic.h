#pragma once

#include <memory>

class Graphic {
	friend std::unique_ptr<Graphic> std::make_unique<Graphic>();
	friend std::unique_ptr<Graphic>::deleter_type;
public:
	Graphic(const Graphic& other) = delete;
	Graphic& operator=(const Graphic& other) = delete;
	static std::unique_ptr<Graphic> GetInstance();
private:

	Graphic();
	virtual ~Graphic();

	static std::unique_ptr<Graphic> instance;
};