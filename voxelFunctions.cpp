//// MINIMUM VOXEL SIZE IS 1/100TH OR 0.01
//double voxelGridCeil(double value, double voxel_size)
//{
//	// Buffer to help with rounding errors when value is a multiple of voxel_size
//	double buffer = -0.00000000000001;
//	value -= value + buffer > 0 ? fmod(value + buffer, voxel_size) - voxel_size : fmod(value + buffer, voxel_size);
//	return value;
//}
//
//double voxelGridFloor(double value, double voxel_size)
//{
//	// Buffer to help with rounding errors when value is a multiple of voxel_size
//	double buffer = 0.00000000000001;
//	value -= value + buffer > 0 ? fmod(value + buffer, voxel_size) : fmod(value + buffer, voxel_size) + voxel_size;
//	return value;
//}
//
//Ray getStartingRay(int pixelX, int pixelY, Vector3 cameraPosition, Vector3 pixel00Position, Vector3 pixelDeltaU, Vector3 pixelDeltaV)
//{
//	// Draw a ray through the center of the pixel from the camera and then march along it
//	Vector3 pixel_sample = pixel00Position
//		+ (pixelX * pixelDeltaU)
//		+ (pixelY * pixelDeltaV);
//
//	return Ray(pixel_sample, pixel_sample - cameraPosition); // Starting position is at the pixel
//	// Starting direction is through pixel sample from origin
//}
//
//Vector3 findStepVector(Vector3 dirSigns, double voxelSize, Vector3 checkPos)
//{
//	// Initialize phase
//	Vector3 next = Vector3(0, 0, 0);
//
//	// Find the step size the ray needs to move to reach the next voxel
//	if (dirSigns.x > 0)
//	{
//		next.x = voxelSize + voxelSize * std::floorf(checkPos.x / voxelSize);
//	}
//	else
//	{
//		next.x = voxelSize * std::ceilf(checkPos.x / voxelSize) - voxelSize;
//	}
//
//	if (dirSigns.y > 0)
//	{
//		next.y = voxelSize + voxelSize * std::floorf(checkPos.y / voxelSize);
//	}
//	else
//	{
//		next.y = voxelSize * std::ceilf(checkPos.y / voxelSize) - voxelSize;
//	}
//
//	if (dirSigns.z > 0)
//	{
//		next.z = voxelSize + voxelSize * std::floorf(checkPos.z / voxelSize);
//	}
//	else
//	{
//		next.z = voxelSize * std::ceilf(checkPos.z / voxelSize) - voxelSize;
//	}
//
//	return next;
//}
//
//Vector3 findMinimumTs(Vector3 nextStepVector, Vector3 rayDir, Vector3 checkPos)
//{
//	Vector3 ts = (nextStepVector - checkPos) / rayDir;
//	Vector3 t_min = Vector3();
//
//	if (ts.x <= ts.y && ts.x <= ts.z)
//	{
//		t_min.x = ts.x;
//	}
//	if (ts.y <= ts.x && ts.y <= ts.z)
//	{
//		t_min.y = ts.y;
//	}
//	if (ts.z <= ts.x && ts.z <= ts.y)
//	{
//		t_min.z = ts.z;
//	}
//
//	if (t_min.x < 0 || t_min.y < 0 || t_min.z < 0)
//	{
//		std::cerr << "Error finding minimum t to move ray: t_min = " << t_min << std::endl;
//	}
//
//	return t_min;
//}
//
//Vector3 convertPositionToVoxelIndex(Vector3 pos, Chunk chunk)
//{
//	return Vector3(
//		round((pos.x - chunk.getVoxelOffset().x) / chunk.getVoxelSize()),
//		round((pos.y - chunk.getVoxelOffset().y) / chunk.getVoxelSize()),
//		round((pos.z - chunk.getVoxelOffset().z) / chunk.getVoxelSize())
//	);
//}
//
//Vector3 convertVoxelIndexToPosition(Vector3 index, Chunk chunk)
//{
//	return (index * chunk.getVoxelSize()) + chunk.getVoxelOffset();
//}
//
//Vector3 findNextVoxelIndex(Vector3 dirSigns, Vector3 tMove, Vector3 checkPos, Chunk chunk)
//{
//	Vector3 nextVoxelIndex = Vector3(
//		voxelGridFloor(checkPos.x, chunk.getVoxelSize()),
//		voxelGridFloor(checkPos.y, chunk.getVoxelSize()),
//		voxelGridFloor(checkPos.z, chunk.getVoxelSize())
//	);
//	if (tMove.x > 0)
//	{
//		nextVoxelIndex.x -= dirSigns.x > 0 ? 0 : chunk.getVoxelSize();
//	}
//	if (tMove.y > 0)
//	{
//		nextVoxelIndex.y -= dirSigns.y > 0 ? 0 : chunk.getVoxelSize();
//	}
//	if (tMove.z > 0)
//	{
//		nextVoxelIndex.z -= dirSigns.z > 0 ? 0 : chunk.getVoxelSize();
//	}
//
//	// Convert to index within the chunk
//	return convertPositionToVoxelIndex(nextVoxelIndex, chunk);
//}
//
//bool checkValidVoxelIndex(Vector3 voxelIndex, Chunk chunk)
//{
//	return (
//		voxelIndex.x >= 0
//		&& voxelIndex.x < chunk.getMaxVoxelIndex()
//		&& voxelIndex.y >= 0
//		&& voxelIndex.y < chunk.getMaxVoxelIndex()
//		&& voxelIndex.z >= 0
//		&& voxelIndex.z < chunk.getMaxVoxelIndex()
//		);
//}


//void renderVoxels(SDL_Renderer* renderer, Chunk chunk)
//{
//	//Create camera
//	Camera camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2.0, 1.0, Vector3(0, 0, 0));
//
//	// Max march distance
//	double t_max = 100.0;
//
//	// On the 25th pixel row from the very bottom
//	bool hitThisRow = false;
//	int afterHitPrinted = 0;
//
//	for (int pixel_y = 0; pixel_y <= SCREEN_HEIGHT; pixel_y++)
//	{
//		std::cout << "Scanlines remaining: " << SCREEN_HEIGHT - pixel_y << std::endl;
//		for (int pixel_x = 0; pixel_x <= SCREEN_WIDTH; pixel_x++)
//		{
//			Ray ray = getStartingRay(pixel_x, pixel_y, camera.getPosition(), camera.getPixel00Position(),
//				camera.getPixelDeltaU(), camera.getPixelDeltaV());
//
//			// Hit flag and starting t
//			bool hit = false;
//			double t = 0.0;
//
//			// Starting position
//			Vector3 checkPos = ray.origin;
//			Vector3 dirSigns = Vector3(0, 0, 0);
//			dirSigns.x = ray.direction.x >= 0 ? 1 : -1;
//			dirSigns.y = ray.direction.y >= 0 ? 1 : -1;
//			dirSigns.z = ray.direction.z >= 0 ? 1 : -1;
//
//			// Entered the chunk flag
//			bool entered = false;
//
//			while (!hit && t <= t_max)
//			{
//				// Initialize phase
//				Vector3 next = findStepVector(dirSigns, chunk.getVoxelSize(), checkPos);
//
//				// Find the t's needed to move the ray this far, and keep the shortest
//				Vector3 tMove = findMinimumTs(next, ray.direction, checkPos);
//
//				// Checking phase
//				t += tMove.x > 0 ? tMove.x : (tMove.y > 0 ? tMove.y : tMove.z);
//				checkPos = ray.origin + ray.direction * t;
//				Vector3 voxelIndexToCheck = findNextVoxelIndex(dirSigns, tMove, checkPos, chunk);
//
//				// Check if point is within chunk (voxel index should be valid)
//				if (checkValidVoxelIndex(voxelIndexToCheck, chunk))
//				{
//					entered = true;
//					// If there is a voxel at that point, then the ray hits
//					if (chunk(voxelIndexToCheck.x, voxelIndexToCheck.y, voxelIndexToCheck.z) != 0)
//					{
//						// Check side of voxel thats being drawn
//						if (tMove.x != 0)
//						{
//							// Entering side of voxel
//							SDL_SetRenderDrawColor(renderer, 80, 10, 175, 255);
//						}
//						else if (tMove.y != 0)
//						{
//							// Entering top or bottom of voxel
//							SDL_SetRenderDrawColor(renderer, 130, 70, 240, 255);
//						}
//						else
//						{
//							// Entering front or back of voxel
//							SDL_SetRenderDrawColor(renderer, 100, 30, 200, 255);
//						}
//
//						// Draw the pixel
//						SDL_RenderDrawPoint(renderer, pixel_x, pixel_y);
//						hit = true;
//					}
//				}
//				// If the chunk has been entered and then exited, break the while loop
//				else if (entered)
//				{
//					break;
//				}
//			}
//		}
//	}
//}


//class Camera
//{
//private:
//	int screenHeight;
//	int screenWidth;
//	double viewportHeight;
//	double viewportWidth;
//	double focalLength;
//	Vector3 position;
//	Vector3 viewportU;
//	Vector3 viewportV;
//	Vector3 pixelDeltaU;
//	Vector3 pixelDeltaV;
//	Vector3 viewportUpperLeft;
//	Vector3 pixel00Position;
//
//public:
//	// Constructor
//	Camera(int screenWidth, int screenHeight, double viewportHeight, double focalLength, Vector3 startingPos = Vector3(0, 0, 0)) :
//		screenWidth(screenWidth), screenHeight(screenHeight), viewportHeight(viewportHeight), focalLength(focalLength),
//		position(startingPos)
//	{
//		viewportWidth = viewportHeight * ((double)screenWidth / screenHeight);
//		viewportU = Vector3(viewportWidth, 0, 0);
//		viewportV = Vector3(0, -viewportHeight, 0);
//		pixelDeltaU = Vector3(viewportWidth / screenWidth, 0, 0);
//		pixelDeltaV = Vector3(0, -viewportHeight / screenHeight, 0);
//		viewportUpperLeft = position
//			+ Vector3(0, 0, focalLength)
//			- viewportU / 2
//			- viewportV / 2;
//		pixel00Position = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
//	}
//
//	// Setters
//	void setScreenDimensions(int screenWidth, int screenHeight)
//	{
//		this->screenWidth = screenWidth;
//		this->screenHeight = screenHeight;
//
//		viewportWidth = viewportHeight * ((double)screenWidth / screenHeight);
//		viewportU = Vector3(viewportWidth, 0, 0);
//		pixelDeltaU = Vector3(viewportWidth / screenWidth, 0, 0);
//		pixelDeltaV = Vector3(0, -viewportHeight / screenHeight, 0);
//		viewportUpperLeft = position
//			+ Vector3(0, 0, focalLength)
//			- viewportU / 2
//			- viewportV / 2;
//		pixel00Position = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
//	}
//
//	void setViewportHeight(double viewportHeight)
//	{
//		this->viewportHeight = viewportHeight;
//
//		viewportWidth = viewportHeight * ((double)screenWidth / screenHeight);
//		viewportU = Vector3(viewportWidth, 0, 0);
//		viewportV = Vector3(0, -viewportHeight, 0);
//		pixelDeltaU = Vector3(viewportWidth / screenWidth, 0, 0);
//		pixelDeltaV = Vector3(0, -viewportHeight / screenHeight, 0);
//		viewportUpperLeft = position
//			+ Vector3(0, 0, focalLength)
//			- viewportU / 2
//			- viewportV / 2;
//		pixel00Position = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
//	}
//
//	void setFocalLength(double focalLength)
//	{
//		this->focalLength = focalLength;
//
//		viewportUpperLeft = position
//			+ Vector3(0, 0, focalLength)
//			- viewportU / 2
//			- viewportV / 2;
//		pixel00Position = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
//	}
//
//	void setCameraPosition(Vector3 position)
//	{
//		this->position = position;
//
//		viewportUpperLeft = position
//			+ Vector3(0, 0, focalLength)
//			- viewportU / 2
//			- viewportV / 2;
//		pixel00Position = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);
//	}
//
//
//	// Getters
//	int getScreenWidth() { return screenWidth; }
//	int getScreenHeight() { return screenHeight; }
//	double getViewportHeight() { return viewportHeight; }
//	double getViewportWidth() { return viewportWidth; }
//	double getFocalLength() { return focalLength; }
//	Vector3 getPosition() { return position; }
//	Vector3 getViewportU() { return viewportU; }
//	Vector3 getViewportV() { return viewportV; }
//	Vector3 getPixelDeltaU() { return pixelDeltaU; }
//	Vector3 getPixelDeltaV() { return pixelDeltaV; }
//	Vector3 getViewportUpperLeft() { return viewportUpperLeft; }
//	Vector3 getPixel00Position() { return pixel00Position; }
//
//};