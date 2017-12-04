#ifndef UPDATEREACTOR_HPP
#define UPDATEREACTOR_HPP

namespace sstd {

	class UpdateReactor
	{
	public:
		UpdateReactor();

		static void load();
		static void main();
		DEFINE_ARX_NAME(LR"(_updatereactor)")
	};

}/*namespace sstd*/

#endif // UPDATEREACTOR_HPP
