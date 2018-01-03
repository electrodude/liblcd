#include "crystalfontz.h"
#include "parallax.h"

#include "driver.h"

const struct liblcd_driver *liblcd_driver_get(const char *name)
{
	if (name == NULL) return NULL;

	if (!strcmp(name, "parallax"))
	{
		return liblcd_driver_parallax;
	}

	if (!strcmp(name, "crystalfontz"))
	{
		return liblcd_driver_crystalfontz;
	}

	return NULL;
}
