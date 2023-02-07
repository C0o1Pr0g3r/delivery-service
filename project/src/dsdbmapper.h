#ifndef DSDBMAPPER_H
#define DSDBMAPPER_H

#include <QString>
#include <QDate>

class DsDbMapper
{
public:
	enum class RegionType {
		Region = 1,
		City
	};

	enum class LocalityType {
		City = 1,
		UrbanTypeSettlement,
		Village
	};

	enum class StreetType {
		Alley = 1,
		Boulevard,
		Entry,
		Street,
		Road,
	};

	enum class PaymentMethod
	{
		Cash = 1,
		NonCash
	};

	enum class PaymentTransacionParticipant
	{
		DeliveryService = 1,
		Sender,
		Recipient
	};

	enum class OfficeType
	{
		Postal = 1,
		Cargo
	};

	enum class Post {
		OfficeOperator = 1,
		HeadOfWarehouse = 3
	};

	enum class ClientType {
		Individual = 1,
		Organization
	};

	struct Mapper {
		Mapper() {}

		Mapper getData() { return *this; }
	};

	struct Region : public Mapper {
		int id;
		RegionType regionType;
		QString title;

		Region()
			: id(0),
			  regionType(static_cast<RegionType>(0)),
			  title("") {}

		Region getData() { return *this; }
	};

	struct District : public Mapper {
		int id;
		Region region;
		QString title;

		District()
			: id(0),
			  region(),
			  title("") {}

		District getData() { return *this; }
	};

	struct Locality : public Mapper {
		int id;
		District district;
		LocalityType localityType;
		QString title;

		Locality()
			: id(0),
			  district(),
			  localityType(static_cast<LocalityType>(0)),
			  title("") {}

		Locality getData() { return *this; }
	};

	struct Street : public Mapper {
		int id;
		Locality locality;
		StreetType streetType;
		QString title;

		Street()
			: id(0),
			  locality(),
			  streetType(static_cast<StreetType>(0)),
			  title("") {}

		Street getData() { return *this; }
	};

	struct Address : public Mapper {
		int id;
		Street street;
		QString houseNumber;

		Address()
			: id(0),
			  street(),
			  houseNumber("") {}

		Address getData() { return *this; }
	};

	struct Office : public Mapper {
		int id;
		int number;
		OfficeType type;
		Address address;
		QString type_str;
		QString address_str;
		QString loc_str;

		Office()
			: id(0),
			  number(0),
			  type(static_cast<OfficeType>(0)),
			  address(),
			  type_str(""),
			  address_str(""),
			  loc_str("") {}

		Office getData() { return *this; }
	};

	struct Employee : public Mapper {
		int id;
		QString lastname;
		QString firstname;
		QString patronymic;
		QString dateOfBirth;
		QString mobile_phone;
		Post post;
		Office office;
		QString LFP;
		QString post_str;

		Employee()
			: id(0),
			  lastname(""),
			  firstname(""),
			  patronymic(""),
			  dateOfBirth(""),
			  mobile_phone(""),
			  post(static_cast<Post>(0)),
			  office(),
			  LFP(""),
			  post_str("") {}

		Employee getData() { return *this; }
	};

	struct Individual : public Mapper {
		int id;
		QString lastname;
		QString firstname;
		QString patronymic;
		QString dateOfBirth;
		QString mobile_phone;
		QString email;

		Individual()
			: id(0),
			  lastname(""),
			  firstname(""),
			  patronymic(""),
			  dateOfBirth(""),
			  mobile_phone("") {}

		Individual getData() { return *this; }
	};

	struct Organization : public Mapper {
		int id;
		QString title;
		QString USREOU;
		Address address;
		QString dateOfBirth;
		QString mobile_phone;
		QString email;
		QString address_str;

		Organization()
			: id(0),
			  title(""),
			  USREOU(""),
			  address(),
			  dateOfBirth(""),
			  mobile_phone(""),
			  email(""),
			  address_str("") {}

		Organization getData() { return *this; }
	};

	struct Client : public Mapper {
		int id;
		ClientType type;
		Individual individual_id;
		Organization organization_id;

		Client() :
			id(0),
			type(static_cast<ClientType>(0)),
			individual_id(),
			organization_id() {}

		Client getData() { return *this; }
	};

	struct Mailing : public Mapper {
		int id;
		QString waybillNumber;
		Client sender;
		Client recipient;
		Office dispatchOffice;
		Office arrivalOffice;
		int currentLocality;

		Mailing()
			: id(0),
			  sender(),
			  recipient(),
			  dispatchOffice(),
			  arrivalOffice(),
			  currentLocality(0) {}

		Mailing getData() { return *this; }
	};
};

#endif // DSDBMAPPER_H
