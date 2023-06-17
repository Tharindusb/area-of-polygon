#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct Point
{
	double x;
	double y;
};


//	calculate the area of the polygon based on its vertices.
class Polygon
{
private:
	std::vector<Point> points_;

public:
	Polygon(const std::vector<Point>& points)
	{
		points_ = points;
	}

	double calculateArea() const 
	{
		int numPoints = points_.size();
		double sum = 0.0;

		for (int i = 0; i < numPoints; ++i) 
		{
			double x1 = points_[i].x;
			double y1 = points_[i].y;
			double x2 = points_[(i + 1) % numPoints].x;
			double y2 = points_[(i + 1) % numPoints].y;
			sum += (x1 * y2) - (x2 * y1);
		}

		double area = 0.5 * std::abs(sum);
		return area;
	}
};

// This class represents a geometric shape defined by a name and a collection of points.
// This provides methods to retrieve the name and the points of the shape.
class Shape {
private:
	std::string name_;
	std::vector<Point> points_;

public:
	Shape(const std::string& name, const std::vector<Point>& points)
	{
		name_ = name;
		points_ = points;
	}

	const std::string& getName() const 
	{
		return name_;
	}

	const std::vector<Point>& getPoints() const 
	{
		return points_;
	}

};

// This class provides methods to determine overlapping shapes 
// Print the overlapping pairs.
class ShapeOverlap {
public:
	// Checks if a given point is inside a shape.
	bool isPointInsideShape(const Point& point, const Shape& shape) 
	{
		int i, j, nvert = shape.getPoints().size();
		bool inside = false;

		for (i = 0, j = nvert - 1; i < nvert; j = i++) 
		{
			if (((shape.getPoints()[i].y >= point.y) != (shape.getPoints()[j].y >= point.y)) &&
				(point.x <= (shape.getPoints()[j].x - shape.getPoints()[i].x) * (point.y - shape.getPoints()[i].y) /
					(shape.getPoints()[j].y - shape.getPoints()[i].y) + shape.getPoints()[i].x)) 
			{
				inside = !inside;
			}
		}

		return inside;
	}

	// Prints the pairs of overlapping shapes
	void printOverlappingShapes(const std::vector<Shape>& shapes) 
	{
		std::vector<std::pair<std::string, std::string>> overlappingPairs;

		for (size_t i = 0; i < shapes.size(); ++i) 
		{
			const Shape& shape1 = shapes[i];

			for (size_t j = i + 1; j < shapes.size(); ++j) 
			{
				const Shape& shape2 = shapes[j];
				bool overlaps = false;

				for (const Point& point : shape1.getPoints()) 
				{
					if (isPointInsideShape(point, shape2)) 
					{
						overlaps = true;
						break;
					}
				}

				if (overlaps) 
				{
					overlappingPairs.push_back({ shape1.getName(), shape2.getName() });
				}
				else 
				{
					for (const Point& point : shape2.getPoints()) 
					{
						if (isPointInsideShape(point, shape1)) 
						{
							overlappingPairs.push_back({ shape2.getName(), shape1.getName() });
							break;
						}
					}
				}
			}
		}

		if (overlappingPairs.empty()) 
		{
			std::cout << "No overlapping shapes found." << std::endl;
		}
		else 
		{
			std::cout << "Overlapping shape pairs:" << std::endl;
			for (const auto& pair : overlappingPairs) 
			{
				std::cout << pair.first << " overlaps with " << pair.second << std::endl;
			}
		}
	}

};

int main() {
	
	std::string fileName = "data.txt";
	std::ifstream inputFile(fileName);
	std::string line;
	std::string shapeName, coordinatesStr;
	std::vector<Shape> shapes;

	while (std::getline(inputFile, line)) 
	{
		std::stringstream ss(line);

		std::getline(ss, shapeName, ',');

		// Get the coordinates string
		std::getline(ss, coordinatesStr); 

		// Remove [ and ]
		coordinatesStr.erase(std::remove(coordinatesStr.begin(), coordinatesStr.end(), '['), coordinatesStr.end());
		coordinatesStr.erase(std::remove(coordinatesStr.begin(), coordinatesStr.end(), ']'), coordinatesStr.end());

		// Remove parentheses
		coordinatesStr.erase(std::remove(coordinatesStr.begin(), coordinatesStr.end(), '('), coordinatesStr.end());
		coordinatesStr.erase(std::remove(coordinatesStr.begin(), coordinatesStr.end(), ')'), coordinatesStr.end());

		// Replace ',' with whitespace
		std::replace(coordinatesStr.begin(), coordinatesStr.end(), ',', ' ');

		std::vector<Point> points;
		std::stringstream cs(coordinatesStr);

		double x, y;
		while (cs >> x >> y) {
			points.push_back({ x, y });
		}

		Polygon polygon(points);
		double area = polygon.calculateArea();

		Shape shape(shapeName, points);
		shapes.push_back(shape);

		// Print the calculated area
		std::cout << "Area of the polygon:" << shapeName << " = " << area << std::endl 
			<< "----------------------------" << std::endl;
	}

	ShapeOverlap shapeOverlap;
	shapeOverlap.printOverlappingShapes(shapes);

	return 0;
}
