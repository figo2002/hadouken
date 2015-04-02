#include <Hadouken/BitTorrent/TorrentHandle.hpp>

#include <Hadouken/BitTorrent/PeerInfo.hpp>
#include <Hadouken/BitTorrent/TorrentInfo.hpp>
#include <Hadouken/BitTorrent/TorrentStatus.hpp>
#include <libtorrent/peer_info.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <string>

using namespace Hadouken::BitTorrent;

TorrentHandle::TorrentHandle(const libtorrent::torrent_handle& handle)
    : handle_(handle)
{
}

TorrentHandle::TorrentHandle(const TorrentHandle& h)
    : handle_(h.handle_),
      tags_(h.tags_)
{
}

void TorrentHandle::getFileProgress(std::vector<int64_t>& progress) const
{
    std::vector<libtorrent::size_type> p;
    handle_.file_progress(p);

    progress.resize(p.size());

    for (int i = 0; i < p.size(); i++)
    {
        progress[i] = p[i];
    }
}

std::string TorrentHandle::getInfoHash() const
{
    return libtorrent::to_hex(handle_.info_hash().to_string());
}

void TorrentHandle::getPeerInfo(std::vector<PeerInfo>& peers) const
{
    std::vector<libtorrent::peer_info> p;
    handle_.get_peer_info(p);

    peers.empty();

    for (libtorrent::peer_info inf : p)
    {
        peers.push_back(PeerInfo(inf));
    }
}

int TorrentHandle::getQueuePosition() const
{
    return handle_.queue_position();
}

TorrentStatus TorrentHandle::getStatus() const
{
    libtorrent::torrent_status status = handle_.status();
    return TorrentStatus(status);
}

std::unique_ptr<TorrentInfo> TorrentHandle::getTorrentFile() const
{
    if (handle_.torrent_file())
    {
        boost::intrusive_ptr<libtorrent::torrent_info const> info = handle_.torrent_file();
        return std::unique_ptr<TorrentInfo>(new TorrentInfo(*info));
    }

    return std::unique_ptr<TorrentInfo>();
}

bool TorrentHandle::isValid() const
{
    return handle_.is_valid();
}

void TorrentHandle::moveStorage(const std::string& savePath) const
{
    handle_.move_storage(savePath);
}

void TorrentHandle::pause() const
{
    if (handle_.status(0x0).paused) {
        return;
    }

    handle_.auto_managed(false);
    handle_.pause();
}

void TorrentHandle::resume() const
{
    handle_.auto_managed(true);
    handle_.resume();
}

void TorrentHandle::addTag(std::string tag)
{
    if (hasTag(tag)) return;
    tags_.push_back(tag);
}

std::vector<std::string> TorrentHandle::getTags() const
{
    return tags_;
}

void TorrentHandle::removeTag(std::string tag)
{
    if (!hasTag(tag)) return;

    std::vector<std::string>::iterator finder = std::find(tags_.begin(), tags_.end(), tag);

    if (finder != tags_.end())
    {
        tags_.erase(finder);
    }
}

bool TorrentHandle::hasTag(std::string tag)
{
    std::vector<std::string>::iterator finder = std::find(tags_.begin(), tags_.end(), tag);
    return (finder != tags_.end());
}