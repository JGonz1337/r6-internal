#include "../types.hpp"
#include "../utils/utils.hpp"
#include <vector>
#pragma warning( disable : 26451 )

namespace sdk
{
	struct vector3
	{
		float x, y, z;
		vector3 operator -(const vector3 v) { return vector3{ x - v.x, y - v.y, z - v.z }; }
		vector3 operator +(const vector3 v) { return vector3{ x + v.x, y + v.y, z + v.z }; }
		vector3 operator *(const float v) { return vector3{ x * v, y * v, z * v }; }
		float dot(const vector3 v)
		{
			return (x * v.x + y * v.y + z * v.z);
		}
		float distance(const vector3 v)
		{
			auto POWF = [](auto n) { return (n) * (n); };
			return sqrt(POWF(x - v.x) + POWF(y - v.y) + POWF(z - v.z));
		}
	};

	struct vector2
	{
		float x, y;

		template <typename t>
		t as()
		{
			auto buf = t();
			buf[0] = x;
			buf[1] = y;
			return buf;
		}
	};

	class outline_component_
	{
	public:
		inline uintptr_t& state() {
			return *reinterpret_cast<uintptr_t*>(this + 0xB0);
		}
	};
	class actor_
	{
	public:
		vector3 position() {
			return *(vector3*)(this + 0x50);
		}

		outline_component_* outline() {
			auto v = (*(uintptr_t*)(this + 0x1C8) + 0x758A6372B999F66Ai64) ^ 0x5506BD0B7F8954E4i64;
			return (outline_component_*)v;
		}
	};
	class pawn_
	{
	public:
		actor_* actor() {
			return (actor_*)_rotl64((*(uintptr_t*)(this + 24) - 0x275B64C73359300Fi64) ^ 0x2102372E900268A1i64, 46);
		}
	};

	class replication_
	{
	public:
		int team() {
			return *(uint8_t*)(this + 0x850) ^ 0xE0;
		}

	};

	class player
	{
	public:
		pawn_* pawn() {
			return (pawn_*)(*(uintptr_t*)(this + 0x38) ^ 0xA5A9F4BA79A2A1C6);
		}
		replication_* replication() {
			return (replication_*)_rotl64(_rotl64(*(uintptr_t*)(this + 0x90), 0x17) - 0x73, 0x1D);
		}
	};

	class game_manager
	{
	public:
		inline static game_manager* get() {
			return (game_manager*)_rotl64((*(uintptr_t*)(O_BASE + 0x7A4BE60) - 0x2DFBC9ECA85C0230) ^ 0x8776BB84854170C4, 0x37);
		}

		std::vector<player*> get_players() {
			const auto decryption = [&](const uint32_t offset = 0) -> uintptr_t
			{
				const auto temp = *(uintptr_t*)(this + offset + 0xD0);
				return ((temp - 0x5B) ^ 0x13) - 0x770AEB7514380744;
			};

			auto start = decryption();
			auto size = static_cast<uint32_t>(decryption(8) & 0x3FFFFFFF);
			auto buf = std::vector<player*>();

			for (uint32_t i = 0; i < size; i++)
			{
				auto address = *(uintptr_t*)(start + (i * 0x8));
				if (utils::get().is_valid(address))
					buf.push_back((player*)address);;
			}
			return buf;
		}
	};

	class camera
	{
	public:
		static camera* get() {
			return (camera*)(*(uintptr_t*)(_rotl64(*(uintptr_t*)(*(uintptr_t*)(O_BASE + 0x676AE78) + 0x40), 0x1) + 0x4C869499115F734A));
		}

		vector2 res() {
			auto rect = RECT();
			GetWindowRect(FindWindow(0, "Rainbow Six"), &rect);

			float width = rect.right - rect.left;
			float height = rect.bottom - rect.top;
			return { width, height };
		}
		vector2 w2s(vector3 world) {
			uintptr_t crypt_xor = 0x2C4271B184C37EFD;
			uintptr_t crypt_add = 0xEFB8CBEAB4138A79;
			uintptr_t right = 0x1D0;
			uintptr_t fov_x = 0x2FC;
			uintptr_t fov_y = 0x300;

			auto decrypt_view_data = [&](const uintptr_t x)
			{
				const uintptr_t temp = (x + crypt_add) ^ crypt_xor;

				crypt_xor -= 0x633FE7A7F870CD7F;
				crypt_add += 0xFC232120A243611;

				return temp << 0x27 | temp >> 0x19;
			};

			auto fovX_ = (*(uint32_t*)((uintptr_t)this + fov_x) + 0x5A90F23D) ^ 0x18;
			auto fovY_ = _rotr(*(uint32_t*)((uintptr_t)this + fov_y) - 0x6A, 1) - 0x0E6DA57D2;

			auto pRight = *(__m128i*)((uintptr_t)this + right);
			pRight.m128i_i64[0] = decrypt_view_data(pRight.m128i_i64[0]);
			pRight.m128i_i64[1] = decrypt_view_data(pRight.m128i_i64[1]);
			auto pUp = *(__m128i*)((uintptr_t)this + right + 0x10);
			pUp.m128i_i64[0] = decrypt_view_data(pUp.m128i_i64[0]);
			pUp.m128i_i64[1] = decrypt_view_data(pUp.m128i_i64[1]);

			auto pForward = *(__m128i*)((uintptr_t)this + right + 0x10 + 0x10);
			pForward.m128i_i64[0] = decrypt_view_data(pForward.m128i_i64[0]);
			pForward.m128i_i64[1] = decrypt_view_data(pForward.m128i_i64[1]);

			auto pTranslation = *(__m128i*)((uintptr_t)this + right + 0x10 + 0x10 + 0x10);
			pTranslation.m128i_i64[0] = decrypt_view_data(pTranslation.m128i_i64[0]);
			pTranslation.m128i_i64[1] = decrypt_view_data(pTranslation.m128i_i64[1]);

			vector3 _right = *(vector3*) & (pRight);
			vector3 _up = *(vector3*) & (pUp);
			vector3 _forward = *(vector3*) & (pForward);
			vector3 _translation = *(vector3*)&(pTranslation);
	 

			auto temp = world - _translation;
			auto x = temp.dot(_right);
			auto y = temp.dot(_up);
			auto z = temp.dot(_forward * -1.f);

			float fovX = *(float*)&fovX_;
			float fovY = *(float*)&fovY_;

			vector2 ret;
			auto screen_size = res();
			float width = screen_size.x;
			float height = screen_size.y;

			ret.x = (width / 2.f) * (1.f + x / -fovX / z);
			ret.y = (height / 2.f) * (1.f - y / -fovY / z);

			if (z >= 1.f && ret.x > 0 && ret.y < width)
				return ret;

			return {};
		}
	};

	enum round_state
	{
		loading = 0,
		picking_ops = 1,
		prep = 2,
		active = 3,
		lobby = 5,
	};
	class round_manager
	{
	public:
		static round_manager* get() {
			return (round_manager*)_rotr64((*(uint64_t*)(O_BASE + 0x758C6D8) ^ 0x12) - 0x74, 0x1);
		}

		int state() {
			return _rotl((*(uint32_t*)(this + 0xF8) ^ 0xFBF03A1D) - 0x7E2A89AB, 0x8);
		}
	};
}