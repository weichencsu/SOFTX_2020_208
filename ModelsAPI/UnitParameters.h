/* Copyright (c) 2020, Dyssol Development Team. All rights reserved. This file is part of Dyssol. See LICENSE file for license information. */

#pragma once

#include "DependentValues.h"
#include "BaseSolver.h"
#include "H5Handler.h"

enum class EUnitParameter
{
	UNKNOWN = 0,
	TIME_DEPENDENT = 1,
	CONSTANT = 2,
	STRING = 3,
	CHECKBOX = 4,
	SOLVER = 5,
	COMBO = 6,
	GROUP = 7,
	COMPOUND = 8
};

#define UP_MIN (-std::numeric_limits<double>::max())
#define UP_MAX  (std::numeric_limits<double>::max())


/* Base class for unit parameters. */
class CBaseUnitParameter
{
	EUnitParameter m_type;                          ///< Type of unit parameter.
	std::string m_name;                             ///< Parameter name.
	std::string m_units;							///< Units of measurement.
	std::string m_description;                      ///< Description of the parameter.

public:
	CBaseUnitParameter();
	explicit CBaseUnitParameter(EUnitParameter _type);
	CBaseUnitParameter(EUnitParameter _type, std::string _name, std::string _units, std::string _description);
	virtual ~CBaseUnitParameter() = default;

	CBaseUnitParameter(const CBaseUnitParameter& _other)            = default;
	CBaseUnitParameter(CBaseUnitParameter&& _other)                 = default;
	CBaseUnitParameter& operator=(const CBaseUnitParameter& _other) = default;
	CBaseUnitParameter& operator=(CBaseUnitParameter&& _other)      = default;

	virtual void Clear() = 0;							     ///< Clears all data.

	EUnitParameter GetType() const;                          ///< Returns parameter type.
	std::string GetName() const;                             ///< Returns parameter name.
	std::string GetUnits() const;                            ///< Returns parameter units.
	std::string GetDescription() const;                      ///< Returns parameter description.

	void SetType(EUnitParameter _type);                      ///< Sets parameter type.
	void SetName(const std::string& _name);                  ///< Sets parameter name.
	void SetUnits(const std::string& _units);                ///< Sets parameter units.
	void SetDescription(const std::string& _description);    ///< Sets parameter description.

	virtual bool IsInBounds() const;	                     ///< Checks whether all values lay in allowed range.

	//virtual void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) = 0;
	//virtual void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path) = 0;
};


class CConstUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	double m_value;                   ///< Const value.
	double m_min;                     ///< Minimum allowed value.
	double m_max;                     ///< Maximum allowed value.

public:
	CConstUnitParameter();
	CConstUnitParameter(std::string _name, std::string _units, std::string _description, double _min, double _max, double _value);

	void Clear() override;            ///< Sets value to zero.

	double GetValue() const;          ///< Returns constant unit parameter value.
	double GetMin() const;            ///< Returns minimum allowed value.
	double GetMax() const;            ///< Returns minimum allowed value.

	void SetValue(double _value);     ///< Sets constant unit parameter value.
	void SetMin(double _min);         ///< Sets minimum allowed value.
	void SetMax(double _max);         ///< Sets minimum allowed value.

	bool IsInBounds() const override; ///< Checks whether m_value lays in range [m_min; m_max].

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


class CTDUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	CDependentValues m_values;                  ///< Time dependent values.
	double m_min;                               ///< Minimum allowed value.
	double m_max;                               ///< Maximum allowed value.

public:
	CTDUnitParameter();
	CTDUnitParameter(std::string _name, std::string _units, std::string _description, double _min, double _max, double _value);

	void Clear() override;                      ///< Removes all values.

	double GetMin() const;                      ///< Returns minimum allowed value.
	double GetMax() const;                      ///< Returns minimum allowed value.

	void SetMin(double _min);                   ///< Sets minimum allowed value.
	void SetMax(double _max);                   ///< Sets minimum allowed value.

	double GetValue(double _time) const;        ///< Returns unit parameter value at given time point using interpolation if necessary.
	void SetValue(double _time, double _value); ///< Adds new unit parameter value at given time point or changes the value of existing one.
	void RemoveValue(double _time);             ///< Removes unit parameter value at given time point if it exists.

	std::vector<double> GetTimes() const;		///< Returns list of all defined time points.
	std::vector<double> GetValues() const;		///< Returns list of all defined values.
	const CDependentValues& GetTDData() const;  ///< Returns the time dependent data itself.

	size_t Size() const;	                    ///< Returns number of defined time points.
	bool IsEmpty() const;	                    ///< Checks whether any time point is defined.
	bool IsInBounds() const override;           ///< Checks whether all m_values lay in range [m_min; m_max].

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


class CStringUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	std::string m_value;                      ///< String parameter value.

public:
	CStringUnitParameter();
	CStringUnitParameter(std::string _name, std::string _description, std::string _value);

	void Clear() override;                    ///< Resets value.

	std::string GetValue() const;             ///< Returns string unit parameter value.
	void SetValue(const std::string& _value); ///< Sets string unit parameter value.

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


class CCheckboxUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	bool m_checked;					///< Check box parameter value: checked/unchecked.

public:
	CCheckboxUnitParameter();
	CCheckboxUnitParameter(std::string _name, std::string _description, bool _checked);

	void Clear() override;          ///< Resets value to unchecked.

	bool IsChecked() const;			///< Returns check box unit parameter value.
	void SetChecked(bool _checked); ///< Sets check box unit parameter value.

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


class CSolverUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	std::string m_key;                      ///< Solver's key.
	ESolverTypes m_solverType;              ///< Solver's type.

public:
	CSolverUnitParameter();
	CSolverUnitParameter(std::string _name, std::string _description, ESolverTypes _type);

	void Clear() override;                  ///< Resets solver's key and type.

	std::string GetKey() const;             ///< Returns solver's key.
	ESolverTypes GetSolverType() const;     ///< Returns solver's type.

	void SetKey(const std::string& _key);   ///< Sets solver's key.
	void SetSolverType(ESolverTypes _type); ///< Sets solver's type.

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


class CComboUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	std::map<size_t, std::string> m_items;			///< List of possible items to select (value:name).
	size_t m_selected = -1;							///< Selected item.

public:
	CComboUnitParameter();
	CComboUnitParameter(std::string _name, std::string _description, size_t _itemDefault, const std::vector<size_t>& _items, const std::vector<std::string>& _itemsNames);

	void Clear() override;							      ///< Resets selected key.

	size_t GetValue() const;						      ///< Returns currently selected item.
	void SetValue(size_t _item);					      ///< Sets new selected item.

	std::vector<size_t> GetItems() const;				  ///< Returns all items.
	std::vector<std::string> GetNames() const;			  ///< Returns all items' names.
	size_t GetItemByName(const std::string& _name) const; ///< Returns item by its name.

	bool HasItem(size_t _item) const;				      ///< Returns true if m_items contains _item.
	bool HasName(const std::string& _name) const;	      ///< Returns true if m_items contains item with _name.

	bool IsInBounds() const override;				      ///< Checks whether m_selected is one of the m_items.

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


// Groups unit parameters.
class CGroupUnitParameter : public CComboUnitParameter
{
	static const unsigned m_cnSaveVersion;

public:
	CGroupUnitParameter();
	CGroupUnitParameter(std::string _name, std::string _description, size_t _itemDefault, const std::vector<size_t>& _items, const std::vector<std::string>& _itemsNames);
};


class CCompoundUnitParameter : public CBaseUnitParameter
{
	static const unsigned m_cnSaveVersion;

	std::string m_key;							///< Unique key of selected compound.

public:
	CCompoundUnitParameter();
	CCompoundUnitParameter(std::string _name, std::string _description);

	void Clear() override;						///< Resets compound's key.

	std::string GetCompound() const;			///< Returns key of currently selected compound.
	void SetCompound(const std::string& _key);	///< Sets new compound's key.

	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path) const;
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);
};


/* Manager of unit parameters for each unit.
 * Each parameter may be a member of one or several groups, to allow showing / hiding of some parameters in GUI.
 * Block is defined by a single CGroupUnitParameter and may have several options to choose.
 * Each option is a group of one or several parameters, which should be shown / hidden together, depending on the selection of corresponding CGroupUnitParameter.
 * One parameter can belong to several groups and several blocks.
 * Block is stored as an index of a CGroupUnitParameter. Group is stored as indices of parameters, which belong to this group.*/
class CUnitParametersManager
{
	static const unsigned m_cnSaveVersion;

	using group_map_t = std::map<size_t, std::map<size_t, std::vector<size_t>>>; // map<iParameter, map<iBlock, vector<iGroups>>>

	std::vector<CBaseUnitParameter*> m_parameters;  ///< All parameters.
	group_map_t m_groups;							///< List of group blocks and corresponding groups, to which parameters belong. Is used to determine activity of parameters.


public:
	CUnitParametersManager() = default;
	CUnitParametersManager(const CUnitParametersManager&)            = delete;
	CUnitParametersManager(CUnitParametersManager&&)                 = delete;
	CUnitParametersManager& operator=(const CUnitParametersManager&) = delete;
	CUnitParametersManager& operator=(CUnitParametersManager&&)      = delete;
	~CUnitParametersManager();

	// Returns number of specified unit parameters.
	size_t ParametersNumber() const;
	// Returns true if unit parameter with given name already exists.
	bool IsNameExist(const std::string& _name) const;

	// Adds new constant unit parameter. If parameter with the given name already exists, does nothing.
	void AddConstParameter(const std::string& _name, const std::string& _units, const std::string& _description, double _min, double _max, double _value);
	// Adds new time-dependent unit parameter. If parameter with the given name already exists, does nothing.
	void AddTDParameter(const std::string& _name, const std::string& _units, const std::string& _description, double _min, double _max, double _value);
	// Adds new string unit parameter. If parameter with the given name already exists, does nothing.
	void AddStringParameter(const std::string& _name, const std::string& _description, const std::string& _value);
	// Adds new check box unit parameter. If parameter with the given name already exists, does nothing.
	void AddCheckboxParameter(const std::string& _name, const std::string& _description, bool _value);
	// Adds new solver unit parameter. If parameter with the given name already exists, does nothing.
	void AddSolverParameter(const std::string& _name, const std::string& _description, ESolverTypes _type);
	// Adds new combo unit parameter. If parameter with the given name already exists, does nothing.
	void AddComboParameter(const std::string& _name, const std::string& _description, size_t _itemDefault, const std::vector<size_t>& _items, const std::vector<std::string>& _itemsNames);
	// Adds new group unit parameter. If parameter with the given name already exists, does nothing.
	void AddGroupParameter(const std::string& _name, const std::string& _description, size_t _itemDefault, const std::vector<size_t>& _items, const std::vector<std::string>& _itemsNames);
	// Adds new compound unit parameter. If parameter with the given name already exists, does nothing.
	void AddCompoundParameter(const std::string& _name, const std::string& _description);

	// Returns list of all defined parameters.
	std::vector<CBaseUnitParameter*> AllParameters() const;

	// Returns pointer to the unit parameter with the specified _index.
	const CBaseUnitParameter* GetParameter(size_t _index) const;
	// Returns const pointer to the unit parameter with the specified _index.
	CBaseUnitParameter* GetParameter(size_t _index);
	// Returns pointer to the unit parameter with the specified _name.
	const CBaseUnitParameter* GetParameter(const std::string& _name) const;
	// Returns const pointer to the unit parameter with the specified _name.
	CBaseUnitParameter* GetParameter(const std::string& _name);

	// Returns const pointer to the const unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CConstUnitParameter* GetConstParameter(size_t _index) const;
	// Returns pointer to the const unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CConstUnitParameter* GetConstParameter(size_t _index);
	// Returns const pointer to the time-dependent unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CTDUnitParameter* GetTDParameter(size_t _index) const;
	// Returns pointer to the time-dependent unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CTDUnitParameter* GetTDParameter(size_t _index);
	// Returns const pointer to the string unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CStringUnitParameter* GetStringParameter(size_t _index) const;
	// Returns pointer to the string unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CStringUnitParameter* GetStringParameter(size_t _index);
	// Returns const pointer to the check box unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CCheckboxUnitParameter* GetCheckboxParameter(size_t _index) const;
	// Returns pointer to the check box unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CCheckboxUnitParameter* GetCheckboxParameter(size_t _index);
	// Returns const pointer to the solver unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CSolverUnitParameter* GetSolverParameter(size_t _index) const;
	// Returns pointer to the solver unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CSolverUnitParameter* GetSolverParameter(size_t _index);
	// Returns const pointer to the combo unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CComboUnitParameter* GetComboParameter(size_t _index) const;
	// Returns pointer to the combo unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CComboUnitParameter* GetComboParameter(size_t _index);
	// Returns const pointer to the group unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CGroupUnitParameter* GetGroupParameter(size_t _index) const;
	// Returns pointer to the group unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CGroupUnitParameter* GetGroupParameter(size_t _index);
	// Returns const pointer to the compound unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	const CCompoundUnitParameter* GetCompoundParameter(size_t _index) const;
	// Returns pointer to the compound unit parameter with the specified _index. If such parameter does not exist, returns nullptr.
	CCompoundUnitParameter* GetCompoundParameter(size_t _index);

	// Returns const pointer to the const unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CConstUnitParameter* GetConstParameter(const std::string& _name) const;
	// Returns pointer to the const unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CConstUnitParameter* GetConstParameter(const std::string& _name);
	// Returns const pointer to the time-dependent unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CTDUnitParameter* GetTDParameter(const std::string& _name) const;
	// Returns pointer to the time-dependent unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CTDUnitParameter* GetTDParameter(const std::string& _name);
	// Returns const pointer to the string unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CStringUnitParameter* GetStringParameter(const std::string& _name) const;
	// Returns pointer to the string unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CStringUnitParameter* GetStringParameter(const std::string& _name);
	// Returns const pointer to the check box unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CCheckboxUnitParameter* GetCheckboxParameter(const std::string& _name) const;
	// Returns pointer to the check box unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CCheckboxUnitParameter* GetCheckboxParameter(const std::string& _name);
	// Returns const pointer to the solver unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CSolverUnitParameter* GetSolverParameter(const std::string& _name) const;
	// Returns pointer to the solver unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CSolverUnitParameter* GetSolverParameter(const std::string& _name);
	// Returns const pointer to the combo unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CComboUnitParameter* GetComboParameter(const std::string& _name) const;
	// Returns pointer to the combo unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CComboUnitParameter* GetComboParameter(const std::string& _name);
	// Returns const pointer to the group unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CGroupUnitParameter* GetGroupParameter(const std::string& _name) const;
	// Returns pointer to the group unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CGroupUnitParameter* GetGroupParameter(const std::string& _name);
	// Returns const pointer to the compound unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	const CCompoundUnitParameter* GetCompoundParameter(const std::string& _name) const;
	// Returns pointer to the compound unit parameter with the specified _name. If such parameter does not exist, returns nullptr.
	CCompoundUnitParameter* GetCompoundParameter(const std::string& _name);

	// Returns value of a constant unit parameter with the specified _index. If such parameter does not exist or is not a constant parameter, returns 0.
	double GetConstParameterValue(size_t _index) const;
	// Returns value of a TD unit parameter with the specified _index and _time. If such parameter does not exist or is not a TD parameter, returns 0.
	double GetTDParameterValue(size_t _index, double _time) const;
	// Returns value of a string unit parameter with the specified _index. If such parameter does not exist or is not a string parameter, returns "".
	std::string GetStringParameterValue(size_t _index) const;
	// Returns value of a check box unit parameter with the specified _index. If such parameter does not exist or is not a check box parameter, returns false.
	bool GetCheckboxParameterValue(size_t _index) const;
	// Returns value of a solver unit parameter with the specified _index. If such parameter does not exist or is not a solver parameter, returns "".
	std::string GetSolverParameterValue(size_t _index) const;
	// Returns value of a combo unit parameter with the specified _index. If such parameter does not exist or is not a combo parameter, returns -1.
	size_t GetComboParameterValue(size_t _index) const;
	// Returns value of a group unit parameter with the specified _index. If such parameter does not exist or is not a group parameter, returns -1.
	size_t GetGroupParameterValue(size_t _index) const;
	// Returns value of a compound unit parameter with the specified _index. If such parameter does not exist or is not a compound parameter, returns "".
	std::string GetCompoundParameterValue(size_t _index) const;

	// Returns value of a constant unit parameter with the specified _name. If such parameter does not exist or is not a constant parameter, returns 0.
	double GetConstParameterValue(const std::string& _name) const;
	// Returns value of a TD unit parameter with the specified _name and _time. If such parameter does not exist or is not a TD parameter, returns 0.
	double GetTDParameterValue(const std::string& _name, double _time) const;
	// Returns value of a string unit parameter with the specified _name. If such parameter does not exist or is not a string parameter, returns "".
	std::string GetStringParameterValue(const std::string& _name) const;
	// Returns value of a check box unit parameter with the specified _name. If such parameter does not exist or is not a check box parameter, returns "".
	bool GetCheckboxParameterValue(const std::string& _name) const;
	// Returns value of a solver unit parameter with the specified _name. If such parameter does not exist or is not a solver parameter, returns "".
	std::string GetSolverParameterValue(const std::string& _name) const;
	// Returns value of a combo unit parameter with the specified _name. If such parameter does not exist or is not a combo parameter, returns -1.
	size_t GetComboParameterValue(const std::string& _name) const;
	// Returns value of a group unit parameter with the specified _name. If such parameter does not exist or is not a group parameter, returns -1.
	size_t GetGroupParameterValue(const std::string& _name) const;
	// Returns value of a compound unit parameter with the specified _name. If such parameter does not exist or is not a compound parameter, returns "".
	std::string GetCompoundParameterValue(const std::string& _name) const;

	// Returns pointers to all specified solver unit parameters.
	std::vector<const CSolverUnitParameter*> GetAllSolverParameters() const;
	// Returns a sorted list of time points form given interval defined in all unit parameters.
	std::vector<double> GetAllTimePoints(double _tBeg, double _tEnd) const;

	// Adds the list of _parameters by their indices to existing _group of existing _block. If _block, _group or some of parameters do not exist, does nothing.
	void AddParametersToGroup(size_t _block, size_t _group, const std::vector<size_t>& _parameters);
	// Adds the list of _parameters by their names to existing _group of existing _block. If _block, _group or some of parameters do not exist, does nothing.
	void AddParametersToGroup(const std::string& _block, const std::string& _group, const std::vector<std::string>& _parameters);
	// Returns true if unit parameter with the specified _index is selected in at least one group of any block, or if it is a not grouped parameter.
	bool IsParameterActive(size_t _index) const;
	// Returns true if this parameter is selected in at least one group of any block, or if it is a not grouped parameter.
	bool IsParameterActive(const CBaseUnitParameter& _parameter) const;

	// Save all parameters to HDF5 file.
	void SaveToFile(CH5Handler& _h5Saver, const std::string& _path);
	// Load all parameters from HDF5 file.
	void LoadFromFile(CH5Handler& _h5Loader, const std::string& _path);

private:
	// Makes parameter with index _parameter a member of the corresponding _block and _group.
	void AddToGroup(size_t _parameter, size_t _block, size_t _group);

	// Returns index of a unit parameter with the given name.
	size_t Name2Index(const std::string& _name) const;
	// Returns indices of unit parameter with the given names.
	std::vector<size_t> Name2Index(const std::vector<std::string>& _names) const;
};
