#include "JSONPathReader.h"
#include "JSON.h"
#include "JSONValue.h"
#include "DistanceVelocityConstraint.h"
#include "LocationVelocityConstraint.h"
#include "CentripetalAccelerationConstraint.h"
#include <fstream>
#include <iostream>

namespace xero
{
	namespace paths
	{
		std::wstring JSONPathReader::toWide(const std::string& str)
		{
			std::wstring result;

			for (char ch : str)
				result += (wchar_t)ch;

			return result;
		}

		std::string JSONPathReader::toNarrow(const std::wstring& str)
		{
			std::string result;

			for (wchar_t ch : str)
				result += (char)ch;

			return result;
		}

		bool JSONPathReader::readFile(const std::string& filename, std::wstring& data)
		{
			std::string line;

			std::ifstream strm(filename);
			if (!strm.is_open()) 
			{
				std::cerr << "pathgen: cannot open path file '" << filename << "'' for reading" << std::endl;
				return false;
			}

			while (getline(strm, line))
			{
				data += toWide(line);
				data += (wchar_t)'\n';
			}

			return true;
		}

		bool JSONPathReader::readJSONPathFile(const std::string& filename, PathCollection& paths)
		{
			std::wstring data;
			bool ret = true;

			if (!readFile(filename, data))
				return false;

			JSONValue* value = JSON::Parse(data.c_str());
			if (value == nullptr) 
			{
				std::cerr << "pathgen: cannot parse file '" << filename << "' as a JSON file" << std::endl;
				return false;
			}

			if (!value->IsObject()) 
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         top level entity in file is not a JSON object" << std::endl;
				return false;
			}

			JSONObject root = value->AsObject();

			if (root.find(L"groups") == root.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         top level object in file does not contains the entry 'groups'" << std::endl;
				return false;
			}

			if (!root[L"groups"]->IsArray())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         the 'groups' entry in top level object is not an JSON array" << std::endl;
				return false;
			}

			JSONArray groups = root[L"groups"]->AsArray();
			for (size_t i = 0; i < groups.size(); i++)
			{
				if (!parseGroup(filename, paths, groups[i]))
					ret = false;
			}

			return ret;
		}

		bool JSONPathReader::parseGroup(const std::string& filename, PathCollection& paths, JSONValue* obj)
		{
			std::string name;
			bool ret = true;


			if (!obj->IsObject())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         an element of the 'groups' entry is not an object" << std::endl;
				return false;
			}

			JSONObject group = obj->AsObject();

			if (group.find(L"name") == group.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         group object does not contain 'name' field" << std::endl;
				return false;
			}

			if (!group[L"name"]->IsString())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         group object 'name' field is not a string" << std::endl;
				return false;
			}

			name = toNarrow(group[L"name"]->AsString());

			if (group.find(L"paths")== group.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         group object does not contain 'paths' field" << std::endl;
				return false;
			}

			if (!group[L"paths"]->IsArray())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         group object 'paths' field is not an array" << std::endl;
				return false;
			}

			JSONArray parray = group[L"paths"]->AsArray();
			for (size_t i = 0; i < parray.size(); i++)
			{
				JSONValue* path = parray[i];
				if (!parsePath(filename, paths, name, path))
					ret = false;
			}


			return ret;
		}


		bool JSONPathReader::parsePath(const std::string& filename,  PathCollection& paths, const std::string& group, JSONValue* obj)
		{
			std::string name;
			std::vector<Pose2d> points;
			std::shared_ptr<RobotPath> onepath;
			double point_velocity_location_constraint_halo = 6.0;

			if (!obj->IsObject())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         an element of the 'paths' entry in a 'group' is not an object" << std::endl;
				return false;
			}

			JSONObject path = obj->AsObject();

			if (path.find(L"name") == path.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path object does not contain 'name' field" << std::endl;
				return false;
			}
			if (!path[L"name"]->IsString())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path object 'name' field is not a string" << std::endl;
				return false;
			}

			name = toNarrow(path[L"name"]->AsString());
			onepath = std::make_shared<RobotPath>(name);

			if (path.find(L"startvelocity") == path.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path '" << name << "' is missing 'startvelocity' field" << std::endl;
				return false;
			}

			if (!path[L"startvelocity"]->IsNumber())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path has 'startvelocity' field that is not a number" << std::endl;
				return false;
			}
			onepath->setStartVelocity(path[L"startvelocity"]->AsNumber());

			if (path.find(L"endvelocity") == path.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path is missing 'endvelocity' field" << std::endl;
				return false;
			}

			if (!path[L"endvelocity"]->IsNumber())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path has 'endvelocity' field that is not a number" << std::endl;
				return false;
			}
			onepath->setEndVelocity(path[L"endvelocity"]->AsNumber());

			if (path.find(L"maxvelocity") == path.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path is missing 'maxvelocity' field" << std::endl;
				return false;
			}

			if (!path[L"maxvelocity"]->IsNumber())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path has 'maxvelocity' field that is not a number" << std::endl;
				return false;
			}
			onepath->setMaxVelocity(path[L"maxvelocity"]->AsNumber());

			if (path.find(L"maxacceleration") == path.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path is missing 'maxacceleration' field" << std::endl;
				return false;
			}

			if (!path[L"maxacceleration"]->IsNumber())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path has 'maxacceleration' field that is not a number" << std::endl;
				return false;
			}
			onepath->setMaxAccel(path[L"maxacceleration"]->AsNumber());

			if (path.find(L"constraints") != path.end())
			{
				if (!path[L"constraints"]->IsArray())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         path object 'constraints' field is not an array" << std::endl;
					return false;
				}

				JSONArray conarr = path[L"constraints"]->AsArray();
				for (size_t i = 0; i < conarr.size(); i++)
				{
					JSONValue* conobj = conarr[i];
					if (!conobj->IsObject())
					{
						std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
						std::cerr << "         path object 'constraints' array element is not an object" << std::endl;
						return false;
					}

					JSONObject constr = conobj->AsObject();

					if (constr.find(L"type") == constr.end())
					{
						std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
						std::cerr << "         path object constraint does not contain 'type' field" << std::endl;
						return false;
					}

					if (!constr[L"type"]->IsString())
					{
						std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
						std::cerr << "         path object constraint has 'type' field but it is not a string" << std::endl;
						return false;
					}

					std::string type = toNarrow(constr[L"type"]->AsString());

					if (type == "distance_velocity")
					{
						double distance;
						double velocity;

						if (constr.find(L"distance") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
							return false;
						}

						if (!constr[L"distance"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
							return false;
						}
						distance = constr[L"distance"]->AsNumber();

						if (constr.find(L"velocity") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'velocity' field" << std::endl;
							return false;
						}

						if (!constr[L"velocity"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'velocity' field that is not a number" << std::endl;
							return false;
						}
						velocity = constr[L"velocity"]->AsNumber();
						onepath->addTimingConstraint(std::make_shared<DistanceVelocityConstraint>(distance, velocity));
					}
					else if (type == "centripetal")
					{
						double maxaccel;

						if (constr.find(L"maxaccel") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
							return false;
						}

						if (!constr[L"maxaccel"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
							return false;
						}
						maxaccel = constr[L"maxaccel"]->AsNumber();

						onepath->addTimingConstraint(std::make_shared<CentripetalAccelerationConstraint>(maxaccel));
					}
					else if (type == "location_velocity")
					{
						double x, y;
						double distance;
						double velocity;

						if (constr.find(L"x") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
							return false;
						}

						if (!constr[L"x"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
							return false;
						}
						x = constr[L"x"]->AsNumber();

						if (constr.find(L"y") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
							return false;
						}

						if (!constr[L"y"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
							return false;
						}
						y = constr[L"y"]->AsNumber();

						if (constr.find(L"distance") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
							return false;
						}

						if (!constr[L"distance"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
							return false;
						}
						distance = constr[L"distance"]->AsNumber();

						if (constr.find(L"velocity") == constr.end())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' missing 'velocity' field" << std::endl;
							return false;
						}

						if (!constr[L"velocity"]->IsNumber())
						{
							std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
							std::cerr << "         constraint 'distance_velocity' has 'velocity' field that is not a number" << std::endl;
							return false;
						}
						velocity = constr[L"velocity"]->AsNumber();
						Translation2d loc(x, y);
						onepath->addTimingConstraint(std::make_shared<LocationVelocityConstraint>(loc, distance, velocity));
					}
					else 
					{
						std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
						std::cerr << "         path object constraint with unknown type '" << type << "'" << std::endl;
						return false;
					}
				}
			}

			if (path.find(L"points") == path.end())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path does not contain 'points' field" << std::endl;
				return false;
			}

			if (!path[L"points"]->IsArray())
			{
				std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
				std::cerr << "         path contains 'point' field but it is not an array" << std::endl;
				return false;
			}

			JSONArray ptarr = path[L"points"]->AsArray();
			for (size_t i = 0; i < ptarr.size(); i++)
			{
				JSONValue* v = ptarr[i];
				if (!v->IsObject())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         element in points array is not an object" << std::endl;
					return false;
				}

				JSONObject pt = v->AsObject();
				double x, y;
				double heading;
				double velocity;

				if (pt.find(L"X") == pt.end())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
					return false;
				}

				if (!pt[L"X"]->IsNumber())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
					return false;
				}
				x = pt[L"X"]->AsNumber();

				if (pt.find(L"Y") == pt.end())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
					return false;
				}

				if (!pt[L"Y"]->IsNumber())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
					return false;
				}
				y = pt[L"Y"]->AsNumber();

				if (pt.find(L"Heading") == pt.end())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
					return false;
				}

				if (!pt[L"Heading"]->IsNumber())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
					return false;
				}
				heading = pt[L"Heading"]->AsNumber();

				if (pt.find(L"Velocity") == pt.end())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' missing 'distance' field" << std::endl;
					return false;
				}

				if (!pt[L"Velocity"]->IsNumber())
				{
					std::cerr << "pathgen: file '" << filename << "' is not a valid path file" << std::endl;
					std::cerr << "         constraint 'distance_velocity' has 'distance' field that is not a number" << std::endl;
					return false;
				}
				velocity = pt[L"Velocity"]->AsNumber();

				Rotation2d rot = Rotation2d::fromDegrees(heading);
				Translation2d trans(x, y);
				Pose2d pose(trans, rot);
				onepath->addPoint(pose);

				if (velocity < onepath->getMaxVelocity())
				{
					Translation2d loc(x, y);
					onepath->addTimingConstraint(std::make_shared<LocationVelocityConstraint>(loc, point_velocity_location_constraint_halo, velocity));
				}
			}

			paths.addPath(group, onepath);
			return true;
		}
	}
}
