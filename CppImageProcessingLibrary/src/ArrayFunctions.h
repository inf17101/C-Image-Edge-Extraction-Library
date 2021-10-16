#pragma once


template<typename T, typename W>
bool create2dArray(T*** newArray, W nrows, W ncols)
{
	if (*newArray != nullptr) return false;

	*newArray = new T*[nrows];
	if (*newArray == nullptr) return false;

	for (W r = 0; r < nrows; ++r)
	{
		(*newArray)[r] = new T[ncols];
		if ((*newArray)[r] == nullptr) return false;
	}
	return true;
}

template<typename T, typename W>
void delete2dArray(T*** array, W height)
{
	if (*array == nullptr)
		return;

	for (W i = 0; i < height; ++i)
		delete[] (*array)[i];
	delete  []*array;

	*array = nullptr;
}
